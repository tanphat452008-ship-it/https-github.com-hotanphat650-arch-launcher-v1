package com.byreytiz.game.gui.dialogs;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.byreytiz.game.R;
import com.byreytiz.game.gui.util.MaskedEditText;
import com.byreytiz.game.gui.util.Utils;

import java.util.ArrayList;

public class DialogAdapter extends RecyclerView.Adapter {
    private int mCurrentSelectedPosition = 0;
    private View mCurrentSelectedView;
    private final ArrayList<TextView> mFieldHeaders;
    private final ArrayList<String> mFieldTexts;
    private final ArrayList<ArrayList<TextView>> mFields;
    private OnClickListener mOnClickListener;
    private OnDoubleClickListener mOnDoubleClickListener;

    public interface OnClickListener {
        void onClick(int i, String str);
    }

    public interface OnDoubleClickListener {
        void onDoubleClick();
    }

    public DialogAdapter(ArrayList<String> fields, ArrayList<TextView> fieldHeaders) {
        this.mFieldTexts = fields;
        this.mFieldHeaders = fieldHeaders;
        this.mFields = new ArrayList<>();
    }

    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.sd_dialog_item, parent, false));
    }

    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        onBindViewHolder((ViewHolder) holder, position);
    }

    @Override
public void onBindViewHolder(ViewHolder holder, int position) {

    String[] cols = mFieldTexts.get(position).split("\t");
    int count = Math.min(cols.length, holder.mFields.size());

    for (int i = 0; i < holder.mFields.size(); i++) {
        TextView field = holder.mFields.get(i);

        if (i < count) {
            field.setText(Utils.transfromColors(cols[i].replace("\\t", "")));
            field.setVisibility(View.VISIBLE);
        } else {
            field.setText("");
            field.setVisibility(View.GONE);
        }
    }

    holder.mFieldBg.setVisibility(
            mCurrentSelectedPosition == position ? View.VISIBLE : View.GONE
    );

    holder.getView().setOnClickListener(v -> {
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
    public void updateSizes() {
    if (mFields.isEmpty()) return;

    int columnCount = mFieldHeaders.size();
    int[] max = new int[columnCount];

    for (ArrayList<TextView> row : mFields) {
        for (int j = 0; j < row.size() && j < columnCount; j++) {
            int w = row.get(j).getWidth();
            if (w > max[j]) max[j] = w;
        }
    }

    for (int i = 0; i < columnCount; i++) {
        int hw = mFieldHeaders.get(i).getWidth();
        if (hw > max[i]) max[i] = hw;
    }

    for (ArrayList<TextView> row : mFields) {
        for (int j = 0; j < row.size() && j < columnCount; j++) {
            if (max[j] > 0)
                row.get(j).setWidth(max[j]);
        }
    }

    for (int i = 0; i < columnCount; i++) {
        if (max[i] > 0)
            mFieldHeaders.get(i).setWidth(max[i]);
    }
    }
    public void setOnClickListener(OnClickListener onClickListener) {
        this.mOnClickListener = onClickListener;
    }

    public void setOnDoubleClickListener(OnDoubleClickListener onDoubleClickListener) {
        this.mOnDoubleClickListener = onDoubleClickListener;
    }

    public ArrayList<ArrayList<TextView>> getFields() {
        return this.mFields;
    }

    public int getItemCount() {
        return this.mFieldTexts.size();
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public ImageView mFieldBg;
        public ArrayList<TextView> mFields = new ArrayList<>();
        private final View mView;

        public ViewHolder(View itemView) {
            super(itemView);
            this.mView = itemView;
            this.mFieldBg = (ImageView) itemView.findViewById(R.id.sd_dialog_item_bg);
            ConstraintLayout field = itemView.findViewById(R.id.sd_dialog_item_main);
            for (int i = 1; i < field.getChildCount(); i++) {
                this.mFields.add((TextView) field.getChildAt(i));
            }
        }

        public View getView() {
            return this.mView;
        }
    }
}

