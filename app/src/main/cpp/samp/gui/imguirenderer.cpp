#include <string>
#include <unordered_map>
#include "imguirenderer.h"
#include "uisettings.h"

// ระบบ Cache ขนาด Text (ลดภาระ CPU อย่างมาก)
struct TextSizeCacheKey {
    std::string text;
    float fontSize;
    bool operator==(const TextSizeCacheKey& other) const {
        return fontSize == other.fontSize && text == other.text;
    }
};

namespace std {
    template<> struct hash<TextSizeCacheKey> {
        size_t operator()(const TextSizeCacheKey& k) const {
            return hash<string>()(k.text) ^ hash<float>()(k.fontSize);
        }
    };
}

static std::unordered_map<TextSizeCacheKey, ImVec2> g_TextSizeCache;

ImGuiRenderer::ImGuiRenderer(ImDrawList* draw_list, ImFont* font) {
    m_drawList = draw_list;
    m_font = font;
}

// --- การวาดรูปทรงพื้นฐาน ---

void ImGuiRenderer::drawLine(const ImVec2& a, const ImVec2& b, const ImColor& color, float thickness) {
    m_drawList->AddLine(a, b, color, thickness);
}

void ImGuiRenderer::drawRect(const ImVec2& a, const ImVec2& b, const ImColor& color, bool fill, float thickness) {
    if (fill) m_drawList->AddRectFilled(a, b, color, 0.0f);
    else m_drawList->AddRect(a, b, color, 0.0f, 0, thickness);
}

void ImGuiRenderer::drawRectFilledMulticolor(const ImVec2& a, const ImVec2& b,
                                             const ImColor& c1, const ImColor& c2, const ImColor& c3, const ImColor& c4) {
    m_drawList->AddRectFilledMultiColor(a, b, (ImU32)c1, (ImU32)c2, (ImU32)c3, (ImU32)c4);
}

void ImGuiRenderer::drawTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImColor& color, bool fill, float thickness) {
    if (fill) m_drawList->AddTriangleFilled(a, b, c, color);
    else m_drawList->AddTriangle(a, b, c, color, thickness);
}

void ImGuiRenderer::drawConvexPolyFilled(ImVec2* points, int num_points, const ImColor& color) {
    m_drawList->AddConvexPolyFilled(points, num_points, color);
}

void ImGuiRenderer::pushClipRect(const ImVec2& min, const ImVec2& max, bool intersect) {
    m_drawList->PushClipRect(min, max, intersect);
}

void ImGuiRenderer::popClipRect() {
    m_drawList->PopClipRect();
}

// --- การเรนเดอร์ตัวอักษรแบบ SAMP (รองรับสี {RRGGBB} และ Tab) ---

void ImGuiRenderer::drawText(const ImVec2& pos, const ImColor& color, const char* begin, const char* end, bool outline, float font_size) {
    if (!begin || (end && begin == end)) return;
    float sz = font_size == 0.0f ? m_font->FontSize : font_size;

    if (outline) {
        float os = UISettings::outlineSize();
        // สร้างสีดำที่มี Alpha เดียวกับสีหลัก
        ImU32 outline_col = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, color.Value.w));
        m_drawList->AddText(m_font, sz, {pos.x + os, pos.y}, outline_col, begin, end);
        m_drawList->AddText(m_font, sz, {pos.x - os, pos.y}, outline_col, begin, end);
        m_drawList->AddText(m_font, sz, {pos.x, pos.y + os}, outline_col, begin, end);
        m_drawList->AddText(m_font, sz, {pos.x, pos.y - os}, outline_col, begin, end);
    }
    m_drawList->AddText(m_font, sz, pos, color, begin, end);
}

void ImGuiRenderer::drawText(const ImVec2& pos, const ImColor& color, const std::string& text, bool outlined, float font_size) {
    if (text.empty()) return;

    const char* text_start = text.c_str();
    const char* text_cur = text_start;
    const char* text_end = text_start + text.length();
    float sz = font_size == 0.0f ? m_font->FontSize : font_size;

    ImVec2 pos_cur = pos;
    ImColor color_cur = color;
    const char* sub_start = text_start;

    while (text_cur < text_end) {
        // 1. จัดการ Color Code {RRGGBB}
        if (*text_cur == '{' && text_cur + 7 < text_end && *(text_cur + 7) == '}') {
            if (text_cur > sub_start) {
                drawText(pos_cur, color_cur, sub_start, text_cur, outlined, sz);
                pos_cur.x += calculateTextSize(std::string(sub_start, text_cur - sub_start), sz).x;
            }
            ImVec4 new_col;
            if (processInlineHexColor(text_cur + 1, text_cur + 7, new_col)) {
                color_cur = ImColor(new_col.x, new_col.y, new_col.z, color.Value.w);
            }
            text_cur += 8;
            sub_start = text_cur;
        }
            // 2. จัดการขึ้นบรรทัดใหม่ \n
        else if (*text_cur == '\n') {
            if (text_cur > sub_start) {
                drawText(pos_cur, color_cur, sub_start, text_cur, outlined, sz);
            }
            pos_cur.x = pos.x;
            pos_cur.y += sz;
            text_cur++;
            sub_start = text_cur;
        }
            // 3. จัดการ Tab \t (สำคัญมากสำหรับหน้าต่าง Dialog)
        else if (*text_cur == '\t') {
            if (text_cur > sub_start) {
                drawText(pos_cur, color_cur, sub_start, text_cur, outlined, sz);
                pos_cur.x += calculateTextSize(std::string(sub_start, text_cur - sub_start), sz).x;
            }
            pos_cur.x += sz * 1.5f; // เว้นระยะ Tab 1.5 เท่าของขนาด Font
            text_cur++;
            sub_start = text_cur;
        }
        else {
            text_cur++;
        }
    }

    if (text_cur > sub_start) {
        drawText(pos_cur, color_cur, sub_start, text_cur, outlined, sz);
    }
}

// --- ฟังก์ชันช่วยเหลือ (ความเร็วสูง) ---

ImVec2 ImGuiRenderer::calculateTextSize(const std::string& text, float font_size) {
    if (text.empty()) return {0, 0};
    float sz = font_size == 0.0f ? m_font->FontSize : font_size;

    TextSizeCacheKey key = { text, sz };
    if (g_TextSizeCache.count(key)) return g_TextSizeCache[key];

    // ลบ Color Code ออกก่อนคำนวณขนาดเพื่อให้ตำแหน่งแม่นยำที่สุด
    std::string clean_text;
    clean_text.reserve(text.length());
    for (size_t i = 0; i < text.length(); ++i) {
        if (text[i] == '{' && i + 7 < text.length() && text[i+7] == '}') {
            i += 7; continue;
        }
        clean_text += text[i];
    }

    ImVec2 res = m_font->CalcTextSizeA(sz, FLT_MAX, 0.0f, clean_text.c_str());
    if (g_TextSizeCache.size() > 1000) g_TextSizeCache.clear();
    g_TextSizeCache[key] = res;
    return res;
}

ImVec2 ImGuiRenderer::calculateTextSize(const char* begin, const char* end, float font_size) {
    return m_font->CalcTextSizeA(font_size == 0.0f ? m_font->FontSize : font_size, FLT_MAX, 0.0f, begin, end);
}

bool ImGuiRenderer::processInlineHexColor(const char* start, const char* end, ImVec4& color) {
    auto h2d = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return 0;
    };
    int r = (h2d(start[0]) << 4) | h2d(start[1]);
    int g = (h2d(start[2]) << 4) | h2d(start[3]);
    int b = (h2d(start[4]) << 4) | h2d(start[5]);
    color = ImVec4(r/255.0f, g/255.0f, b/255.0f, 1.0f);
    return true;
}

void ImGuiRenderer::drawImage(const ImVec2& a, const ImVec2& b, ImTextureID tex) {
    m_drawList->AddImage(tex, a, b);
}

bool ImGuiRenderer::isWithinRenderDistance(const ImVec2& p1, const ImVec2& p2, float d) {
    float dx = p1.x - p2.x, dy = p1.y - p2.y;
    return (dx*dx + dy*dy) <= (d*d);
}