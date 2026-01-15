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

public class DialogAdapter extends RecyclerView.Adapter<DialogAdapter.ViewHolder> {
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
        this.mFieldTexts = fields != null ? fields : new ArrayList<>();
        this.mFieldHeaders = fieldHeaders != null ? fieldHeaders : new ArrayList<>();
        this.mFields = new ArrayList<>();
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.sd_dialog_item, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(DialogAdapter.ViewHolder holder, int position) {
        // Bounds check
        if (position < 0 || position >= this.mFieldTexts.size()) {
            return;
        }

        String fieldText = this.mFieldTexts.get(position);
        if (fieldText == null) {
            fieldText = "";
        }

        String[] headers = fieldText.split("\t");
        ArrayList<TextView> fields = new ArrayList<>();

        // Clear previous visibility
        for (TextView field : holder.mFields) {
            field.setVisibility(View.GONE);
        }

        // Set field texts with bounds checking
        for (int i = 0; i < headers.length && i < holder.mFields.size(); i++) {
            TextView field = holder.mFields.get(i);
            if (field != null) {
                String headerText = headers[i].replace("\\t", "");
                field.setText(Utils.transfromColors(headerText));
                field.setVisibility(View.VISIBLE);
                fields.add(field);
            }
        }

        // Ensure mFields list is properly sized
        if (position >= this.mFields.size()) {
            // Add empty lists until we reach the required position
            while (this.mFields.size() <= position) {
                this.mFields.add(new ArrayList<>());
            }
        }
        this.mFields.set(position, fields);

        // Handle selection state
        if (this.mCurrentSelectedPosition == position) {
            ImageView imageView = holder.mFieldBg;
            if (imageView != null) {
                this.mCurrentSelectedView = imageView;
                imageView.setVisibility(View.VISIBLE);
                if (this.mOnClickListener != null && !holder.mFields.isEmpty()) {
                    TextView firstField = holder.mFields.get(0);
                    if (firstField != null) {
                        this.mOnClickListener.onClick(position, firstField.getText().toString());
                    }
                }
            }
        } else {
            if (holder.mFieldBg != null) {
                holder.mFieldBg.setVisibility(View.GONE);
            }
        }

        // Set click listener with null checks
        View itemView = holder.getView();
        if (itemView != null) {
            itemView.setOnClickListener(view -> {
                int adapterPosition = holder.getAdapterPosition();
                if (adapterPosition == RecyclerView.NO_POSITION) {
                    return;
                }

                if (this.mCurrentSelectedPosition != adapterPosition) {
                    // Hide previous selection
                    View previousView = this.mCurrentSelectedView;
                    if (previousView != null) {
                        previousView.setVisibility(View.GONE);
                    }

                    // Set new selection
                    this.mCurrentSelectedPosition = adapterPosition;
                    if (holder.mFieldBg != null) {
                        this.mCurrentSelectedView = holder.mFieldBg;
                        holder.mFieldBg.setVisibility(View.VISIBLE);
                    }

                    // Notify click listener
                    if (this.mOnClickListener != null && !holder.mFields.isEmpty()) {
                        TextView firstField = holder.mFields.get(0);
                        if (firstField != null) {
                            this.mOnClickListener.onClick(adapterPosition, firstField.getText().toString());
                        }
                    }
                } else {
                    // Handle double click
                    OnDoubleClickListener doubleClickListener = this.mOnDoubleClickListener;
                    if (doubleClickListener != null) {
                        doubleClickListener.onDoubleClick();
                    }
                }
            });
        }
    }

    public void updateSizes() {
        if (this.mFields.isEmpty() || this.mFieldHeaders.isEmpty()) {
            return;
        }

        int[] max = new int[4];

        // Calculate max widths for each column
        for (int i = 0; i < this.mFields.size(); i++) {
            ArrayList<TextView> fieldRow = this.mFields.get(i);
            if (fieldRow != null) {
                for (int j = 0; j < fieldRow.size() && j < 4; j++) {
                    TextView field = fieldRow.get(j);
                    if (field != null) {
                        int width = field.getWidth();
                        if (max[j] < width) {
                            max[j] = width;
                        }
                    }
                }
            }
        }

        // Check header widths
        for (int i = 0; i < Math.min(this.mFieldHeaders.size(), 4); i++) {
            TextView header = this.mFieldHeaders.get(i);
            if (header != null) {
                int headerWidth = header.getWidth();
                Log.i("DIALOG", max[i] + "\t" + header.getText() + MaskedEditText.SPACE + headerWidth);
                if (max[i] < headerWidth) {
                    max[i] = headerWidth;
                }
            }
        }

        // Apply max widths to fields
        for (int i = 0; i < this.mFields.size(); i++) {
            ArrayList<TextView> fieldRow = this.mFields.get(i);
            if (fieldRow != null) {
                for (int j = 0; j < fieldRow.size() && j < 4; j++) {
                    TextView field = fieldRow.get(j);
                    if (field != null && j < max.length) {
                        field.setWidth(max[j]);
                    }
                }
            }
        }

        // Apply max widths to headers
        for (int i = 0; i < Math.min(this.mFieldHeaders.size(), 4); i++) {
            TextView header = this.mFieldHeaders.get(i);
            if (header != null && i < max.length) {
                header.setWidth(max[i]);
            }
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

    @Override
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

            // Safe findViewById
            this.mFieldBg = itemView.findViewById(R.id.sd_dialog_item_bg);
            ConstraintLayout field = itemView.findViewById(R.id.sd_dialog_item_main);

            if (field != null) {
                for (int i = 1; i < field.getChildCount(); i++) {
                    View child = field.getChildAt(i);
                    if (child instanceof TextView) {
                        this.mFields.add((TextView) child);
                    }
                }
            }
        }

        public View getView() {
            return this.mView;
        }
    }
}