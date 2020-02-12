package com.airblair.chikuchakutoe;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import androidx.annotation.Nullable;

public class ViewDaBoard extends View {
    private static final int LINE_THICK = 10;
    private static final int SECTION_MARGIN = 20;
    private static final int SECTION_WIDTH = 15;
    private int width, height, sectionW, sectionH;
    private Paint gridPaint, oPaint, xPaint;
    private GameBoard game;
    private MainActivity activity;

    public ViewDaBoard(Context context) {
        super(context);
    }

    public ViewDaBoard(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        gridPaint = new Paint();
        oPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        oPaint.setColor(Color.MAGENTA);
        oPaint.setStyle(Paint.Style.STROKE);
        oPaint.setStrokeWidth(SECTION_WIDTH);
        xPaint = new Paint(oPaint);
        xPaint.setColor(Color.CYAN);
    }

    public void setMainActivity(MainActivity a) {
        activity = a;
    }

    public void setGameEngine(GameBoard g) {
        game = g;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        height = View.MeasureSpec.getSize(heightMeasureSpec);
        width = View.MeasureSpec.getSize(widthMeasureSpec);
        sectionW = (width - LINE_THICK) / 3;
        sectionH = (height - LINE_THICK) / 3;

        setMeasuredDimension(width, height);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        drawGrid(canvas);
        drawBoard(canvas);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (!game.isEnded()  &&  event.getAction() == MotionEvent.ACTION_DOWN) {
            int x = (int) (event.getX() / sectionW);
            int y = (int) (event.getY() / sectionH);
            char win = game.play(x, y);
            invalidate();

            if (win != ' ') {
                activity.end(win);
            } else {
                //Uncomment This line for ai. but ai only picks randim nums
               // win = game.compute();
                invalidate();

                if (win != ' ') {
                    activity.end(win);
                }
            }
        }

        return super.onTouchEvent(event);
    }

    private void drawBoard(Canvas canvas) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                drawElt(canvas, game.getSection(i, j), i, j);
            }
        }
    }

    private void drawGrid(Canvas canvas) {
        for (int i = 0; i < 2; i++) {
            // vertical lines
            float left = sectionW * (i + 1);
            float right = left + LINE_THICK;
            float top = 0;
            float bottom = height;

            canvas.drawRect(left, top, right, bottom, gridPaint);

            // horizontal lines
            float left2 = 0;
            float right2 = width;
            float top2 = sectionH * (i + 1);
            float bottom2 = top2 + LINE_THICK;

            canvas.drawRect(left2, top2, right2, bottom2, gridPaint);
        }
    }

    private void drawElt(Canvas canvas, char c, int x, int y) {
        if (c == 'O') {
            float cx = (sectionW * x) + sectionW / 2;
            float cy = (sectionH * y) + sectionH / 2;

            canvas.drawCircle(cx, cy, Math.min(sectionW, sectionH) / 2 - SECTION_MARGIN * 2, oPaint);

        } else if (c == 'X') {
            float startX = (sectionW * x) + SECTION_MARGIN;
            float startY = (sectionH * y) + SECTION_MARGIN;
            float endX = startX + sectionW - SECTION_MARGIN * 2;
            float endY = startY + sectionH - SECTION_MARGIN;

            canvas.drawLine(startX, startY, endX, endY, xPaint);

            float startX2 = (sectionW * (x + 1)) - SECTION_MARGIN;
            float startY2 = (sectionH * y) + SECTION_MARGIN;
            float endX2 = startX2 - sectionW + SECTION_MARGIN * 2;
            float endY2 = startY2 + sectionH - SECTION_MARGIN;

            canvas.drawLine(startX2, startY2, endX2, endY2, xPaint);
        }
    }

}
