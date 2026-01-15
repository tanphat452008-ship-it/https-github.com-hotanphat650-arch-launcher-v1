package com.byreytiz.game.gui.dialogs;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.byreytiz.game.R;
import com.byreytiz.game.gui.util.Utils;

import java.util.ArrayList;

public class DialogAdapter
        extends RecyclerView.Adapter<DialogAdapter.ViewHolder> {

    private int mCurrentSelectedPosition = -1;
    private View mCurrentSelectedView;

    private final ArrayList<String> mFieldTexts;
    private final ArrayList<TextView> mFieldHeaders;

    // cache rows đã bind để updateSizes()
    private final ArrayList<ArrayList<TextView>> mBoundRows = new ArrayList<>();

    private OnClickListener mOnClickListener;
    private OnDoubleClickListener mOnDoubleClickListener;

    public interface OnClickListener {
        void onClick(int index, String text);
    }

    public interface OnDoubleClickListener {
        void onDoubleClick();
    }

    public DialogAdapter(ArrayList<String> rows,
                         ArrayList<TextView> headers) {
        this.mFieldTexts = rows != null ? rows : new ArrayList<>();
        this.mFieldHeaders = headers;
    }

    // ================= ADAPTER =================

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.sd_dialog_item, parent, false);
        return new ViewHolder(v);
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {

        String[] cols = mFieldTexts.get(position).split("\t");
        int count = Math.min(cols.length, holder.mFields.size());

        for (int i = 0; i < holder.mFields.size(); i++) {
            TextView tv = holder.mFields.get(i);
            if (i < count) {
                tv.setText(Utils.transfromColors(cols[i]));
                tv.setVisibility(View.VISIBLE);
            } else {
                tv.setText("");
                tv.setVisibility(View.GONE);
            }
        }

        holder.mFieldBg.setVisibility(
                mCurrentSelectedPosition == position
                        ? View.VISIBLE
                        : View.GONE
        );

        if (!mBoundRows.contains(holder.mFields)) {
            mBoundRows.add(holder.mFields);
        }

        holder.itemView.setOnClickListener(v -> {
            int pos = holder.getAdapterPosition();
            if (pos == RecyclerView.NO_POSITION) return;

            if (mCurrentSelectedPosition != pos) {
                if (mCurrentSelectedView != null)
                    mCurrentSelectedView.setVisibility(View.GONE);

                mCurrentSelectedPosition = pos;
                mCurrentSelectedView = holder.mFieldBg;
                holder.mFieldBg.setVisibility(View.VISIBLE);

                if (mOnClickListener != null) {
                    mOnClickListener.onClick(
                            pos,
                            holder.mFields.get(0).getText().toString()
                    );
                }
            } else if (mOnDoubleClickListener != null) {
                mOnDoubleClickListener.onDoubleClick();
            }
        });
    }

    @Override
    public int getItemCount() {
        return mFieldTexts.size();
    }

    // ================= TABLIST WIDTH =================

    public void updateSizes() {
        if (mBoundRows.isEmpty() || mFieldHeaders == null) return;

        int columnCount = mFieldHeaders.size();
        int[] max = new int[columnCount];

        // rows
        for (ArrayList<TextView> row : mBoundRows) {
            for (int i = 0; i < row.size() && i < columnCount; i++) {
                int w = row.get(i).getWidth();
                if (w > max[i]) max[i] = w;
            }
        }

        // headers
        for (int i = 0; i < columnCount; i++) {
            int hw = mFieldHeaders.get(i).getWidth();
            if (hw > max[i]) max[i] = hw;
        }

        // apply rows
        for (ArrayList<TextView> row : mBoundRows) {
            for (int i = 0; i < row.size() && i < columnCount; i++) {
                if (max[i] > 0) {
                    row.get(i).setWidth(max[i]);
                }
            }
        }

        // apply headers
        for (int i = 0; i < columnCount; i++) {
            if (max[i] > 0) {
                mFieldHeaders.get(i).setWidth(max[i]);
            }
        }
    }

    // ================= LISTENERS =================

    public void setOnClickListener(OnClickListener l) {
        this.mOnClickListener = l;
    }

    public void setOnDoubleClickListener(OnDoubleClickListener l) {
        this.mOnDoubleClickListener = l;
    }

    // ================= VIEW HOLDER =================

    public static class ViewHolder extends RecyclerView.ViewHolder {

        ImageView mFieldBg;
        ArrayList<TextView> mFields = new ArrayList<>();

        public ViewHolder(View itemView) {
            super(itemView);

            mFieldBg = itemView.findViewById(R.id.sd_dialog_item_bg);

            ConstraintLayout layout =
                    itemView.findViewById(R.id.sd_dialog_item_main);

            for (int i = 1; i < layout.getChildCount(); i++) {
                View v = layout.getChildAt(i);
                if (v instanceof TextView) {
                    mFields.add((TextView) v);
                }
            }
        }
    }
}
