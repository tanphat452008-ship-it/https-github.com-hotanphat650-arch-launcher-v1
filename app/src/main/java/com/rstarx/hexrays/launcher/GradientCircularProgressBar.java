package com.rstarx.hexrays.launcher;

import android.animation.ValueAnimator;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;
import android.view.animation.LinearInterpolator;

public class GradientCircularProgressBar extends View {
    private int backgroundColor = 0;
    private Paint paint;
    private int progressColor = Color.parseColor("#FFA500");
    private RectF rectF;
    private ValueAnimator rotateAnimator;
    private float rotationAngle = 0.0f;
    private float strokeWidth = 10.0f;

    public GradientCircularProgressBar(Context context) {
        super(context);
        init();
    }

    public GradientCircularProgressBar(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public GradientCircularProgressBar(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        this.paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        this.paint.setStyle(Paint.Style.STROKE);
        this.paint.setStrokeCap(Paint.Cap.ROUND);
        this.rectF = new RectF();

        // ตั้งค่าการหมุน
        this.rotateAnimator = ValueAnimator.ofFloat(0.0f, 360.0f);
        this.rotateAnimator.setDuration(1000L);
        this.rotateAnimator.setRepeatCount(ValueAnimator.INFINITE); // หมุนไม่สิ้นสุด
        this.rotateAnimator.setInterpolator(new LinearInterpolator());
        this.rotateAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                rotationAngle = (float) animation.getAnimatedValue();
                invalidate(); // วาดใหม่ทุกครั้งที่มุมเปลี่ยน
            }
        });
        this.rotateAnimator.start();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        float size = Math.min(getWidth(), getHeight());
        if (size <= 0) return;

        float inset = this.strokeWidth + 12.0f;
        this.rectF.set(inset, inset, size - inset, size - inset);

        float whiteInset = 10.0f + inset;
        RectF whiteFrame = new RectF(whiteInset, whiteInset, size - whiteInset, size - whiteInset);

        // วาดพื้นหลังวงกลมจางๆ
        this.paint.setStyle(Paint.Style.FILL);
        this.paint.setColor(Color.parseColor("#33000000"));
        canvas.drawOval(whiteFrame, this.paint);

        // วาดขอบเส้นสีขาวด้านใน
        this.paint.setStyle(Paint.Style.STROKE);
        this.paint.setColor(Color.WHITE);
        this.paint.setStrokeWidth(2.0f);
        canvas.drawOval(whiteFrame, this.paint);

        // วาดเส้นรอบนอกสีดำจางๆ
        this.paint.setColor(Color.parseColor("#33000000"));
        this.paint.setStrokeWidth(this.strokeWidth + 6.0f);
        canvas.drawArc(this.rectF, 0, 360.0f, false, this.paint);

        // วาดตัวโหลดสีส้มที่หมุนได้
        canvas.save();
        canvas.rotate(this.rotationAngle, getWidth() / 2f, getHeight() / 2f);
        this.paint.setColor(this.progressColor);
        this.paint.setStrokeWidth(this.strokeWidth);
        canvas.drawArc(this.rectF, -90.0f, 50.0f, false, this.paint);
        canvas.restore();
    }

    public void startSpinning() {
        if (this.rotateAnimator != null && !this.rotateAnimator.isRunning()) {
            this.rotateAnimator.start();
        }
    }

    public void stopSpinning() {
        if (this.rotateAnimator != null) {
            this.rotateAnimator.cancel();
        }
    }
}