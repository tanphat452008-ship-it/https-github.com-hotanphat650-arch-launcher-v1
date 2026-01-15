package com.byreytiz.game.gui.dialogs;

import android.app.Activity;
import android.content.Context;
import android.text.Editable;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import com.nvidia.devtech.CustomEditText;
import com.nvidia.devtech.NvEventQueueActivity;
import com.byreytiz.game.R;
import com.byreytiz.game.gui.util.CustomRecyclerView;
import com.byreytiz.game.gui.util.Utils;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

public class Dialog {
    private static final int DIALOG_LEFT_BTN_ID = 1;
    private static final int DIALOG_RIGHT_BTN_ID = 0;
    private static final int DIALOG_STYLE_INPUT = 1;
    private static final int DIALOG_STYLE_LIST = 2;
    private static final int DIALOG_STYLE_MSGBOX = 0;
    private static final int DIALOG_STYLE_PASSWORD = 3;
    private static final int DIALOG_STYLE_TABLIST = 4;
    private static final int DIALOG_STYLE_TABLIST_HEADER = 5;
    private final TextView mCaption;
    private final TextView mContent;
    private int mCurrentDialogId = -1;
    private int mCurrentDialogTypeId = -1;
    private String mCurrentInputText = "";
    private int mCurrentListItem = -1;
    private final CustomRecyclerView mCustomRecyclerView;
    private final ArrayList<TextView> mHeadersList;
    private final CustomEditText mInput;
    private final ConstraintLayout mInputLayout;
    private final ConstraintLayout mLeftBtn;
    private final ConstraintLayout mListLayout;
    private final ConstraintLayout mMainLayout;
    private final ScrollView mMsgBoxLayout;
    private final ConstraintLayout mRightBtn;
    private ArrayList<String> mRowsList;

    public Dialog(Activity activity) {
        this.mMainLayout = activity.findViewById(R.id.sd_dialog_main);
        this.mCaption = (TextView) activity.findViewById(R.id.sd_dialog_caption);
        this.mContent = (TextView) activity.findViewById(R.id.sd_dialog_text);
        ConstraintLayout findViewById1 = activity.findViewById(R.id.sd_button_positive);
        this.mLeftBtn = findViewById1;
        ConstraintLayout findViewById2 = activity.findViewById(R.id.sd_button_negative);
        this.mRightBtn = findViewById2;
        this.mInputLayout = activity.findViewById(R.id.sd_dialog_input_layout);
        this.mListLayout = activity.findViewById(R.id.sd_dialog_list_layout);
        this.mMsgBoxLayout = (ScrollView) activity.findViewById(R.id.sd_dialog_text_layout);
        this.mInput = (CustomEditText) activity.findViewById(R.id.sd_dialog_input);
        this.mCustomRecyclerView = (CustomRecyclerView) activity.findViewById(R.id.sd_dialog_list_recycler);
        findViewById1.setOnClickListener(view -> sendDialogResponse(1));
        findViewById2.setOnClickListener(view -> sendDialogResponse(0));
        this.mRowsList = new ArrayList<>();
        this.mHeadersList = new ArrayList<>();
        ConstraintLayout mHeadersLayout = activity.findViewById(R.id.sd_dialog_tablist_row);
        for (int i = 0; i < mHeadersLayout.getChildCount(); i++) {
            this.mHeadersList.add((TextView) mHeadersLayout.getChildAt(i));
        }
        this.mInput.setOnEditorActionListener((textView, i, keyEvent) -> {
            Editable editableText;
            if ((i != 6 && i != 5) || (editableText = this.mInput.getText()) == null) {
                return false;
            }
            this.mCurrentInputText = editableText.toString();
            return false;
        });
        this.mInput.setOnClickListener(view ->
        {
            this.mInput.requestFocus();
            ((InputMethodManager) NvEventQueueActivity.getInstance().getSystemService("input_method")).showSoftInput(this.mInput, 1);
        });
        Utils.HideLayout(this.mMainLayout, false);
    }
    public void show(int dialogId, int dialogTypeId,
                 String caption,
                 String content,
                 String leftBtnText,
                 String rightBtnText) {

    clearDialogData();

    mCurrentDialogId = dialogId;
    mCurrentDialogTypeId = dialogTypeId;

    // Null safety
    if (caption == null) caption = "";
    if (content == null) content = "";
    if (leftBtnText == null) leftBtnText = "";
    if (rightBtnText == null) rightBtnText = "";
    mInputLayout.setVisibility(View.GONE);
    mListLayout.setVisibility(View.GONE);
    mMsgBoxLayout.setVisibility(View.GONE);
    if (dialogTypeId == DIALOG_STYLE_MSGBOX) {
        mMsgBoxLayout.setVisibility(View.VISIBLE);

    } else if (dialogTypeId == DIALOG_STYLE_INPUT ||
               dialogTypeId == DIALOG_STYLE_PASSWORD) {

        mInputLayout.setVisibility(View.VISIBLE);
        mMsgBoxLayout.setVisibility(View.VISIBLE);

        mInput.setText("");
        mInput.requestFocus();

    } else { // LIST / TABLIST / TABLIST_HEADER

        mListLayout.setVisibility(View.VISIBLE);
        loadTabList(content);

        if (!mRowsList.isEmpty()) {
            ArrayList<String> fixedRows =
                    Utils.fixFieldsForDialog(mRowsList);
            mRowsList = fixedRows;

            DialogAdapter adapter =
                    new DialogAdapter(fixedRows, mHeadersList);

            adapter.setOnClickListener((i, str) -> {
                if (i < 0 || i >= mRowsList.size()) return;
                mCurrentListItem = i;
                mCurrentInputText = str;
            });

            adapter.setOnDoubleClickListener(() ->
                    sendDialogResponse(DIALOG_LEFT_BTN_ID));

            mCustomRecyclerView.setLayoutManager(
                    new LinearLayoutManager(NvEventQueueActivity.getInstance()));

            mCustomRecyclerView.setAdapter(adapter);

            if (dialogTypeId != DIALOG_STYLE_LIST) {
                mCustomRecyclerView.post(adapter::updateSizes);
            }
        }
    }

    // ---------- TEXT ----------
    mCaption.setText(Utils.transfromColors(caption));
    mContent.setText(Utils.transfromColors(content));

    ((TextView) mLeftBtn.getChildAt(0))
            .setText(Utils.transfromColors(leftBtnText));

    ((TextView) mRightBtn.getChildAt(0))
            .setText(Utils.transfromColors(rightBtnText));
    mRightBtn.setVisibility(
            rightBtnText.isEmpty() ? View.GONE : View.VISIBLE
    );

    Utils.ShowLayout(mMainLayout, true);
}
    public void hideWithoutReset() {
        Utils.HideLayout(this.mMainLayout, false);
    }

    public void showWithOldContent() {
        Utils.ShowLayout(this.mMainLayout, false);
    }
    public void sendDialogResponse(int btnId) {
    if (mInput != null && mInput.getText() != null) {
        mCurrentInputText = mInput.getText().toString();
    }

    InputMethodManager imm =
        (InputMethodManager) NvEventQueueActivity.getInstance()
            .getSystemService(Context.INPUT_METHOD_SERVICE);

    if (imm != null && mInput != null) {
        imm.hideSoftInputFromWindow(mInput.getWindowToken(), 0);
    }

    try {
        NvEventQueueActivity.getInstance().sendDialogResponse(
            btnId,
            mCurrentDialogId,
            mCurrentListItem,
            mCurrentInputText.getBytes("windows-1251")
        );
        Utils.HideLayout(mMainLayout, true);
    } catch (Exception e) {
        e.printStackTrace();
    }
    }
    private void loadTabList(String content) {
    if (content == null || content.isEmpty()) return;

    String[] strings = content.split("\n");

    for (int i = 0; i < strings.length; i++) {
        if (this.mCurrentDialogTypeId == DIALOG_STYLE_TABLIST_HEADER && i == 0) {
            String[] headers = strings[i].split("\t");
            int count = Math.min(headers.length, mHeadersList.size());

            for (int j = 0; j < count; j++) {
                TextView header = mHeadersList.get(j);
                header.setText(Utils.transfromColors(headers[j]));
                header.setVisibility(View.VISIBLE);
            }
        } else {
            mRowsList.add(strings[i]);
        }
    }
    }
    private void clearDialogData() {
    mCurrentInputText = "";
    mCurrentDialogId = -1;
    mCurrentDialogTypeId = -1;
    mCurrentListItem = -1;

    if (mInput != null)
        mInput.setText("");

    mRowsList.clear();

    for (TextView header : mHeadersList) {
        header.setVisibility(View.GONE);
        header.setText("");
    }
    }
    public void onHeightChanged(int height) {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) this.mMainLayout.getLayoutParams();
        params.setMargins(0, 0, 0, height);
        this.mMainLayout.setLayoutParams(params);
    }
}


