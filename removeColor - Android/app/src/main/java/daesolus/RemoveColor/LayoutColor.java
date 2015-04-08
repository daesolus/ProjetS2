package daesolus.RemoveColor;
import 	android.app.Activity;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.view.View;
import android.widget.FrameLayout;
/**
 * Created by DaeSoluS on 2015-04-07.
 */
public class LayoutColor extends FrameLayout implements Runnable{
    private Activity context;
    private View myView;
    private ColorDrawable layers[];
    private ChangeColor backgroundColorAnimation;
    private String colors[];

    public LayoutColor(Activity context,String colors[]) {
        super(context);
        myView = this;
        this.colors = colors;
        this.context = context;
        layers =  new ColorDrawable [2];
        layers[0] = new ColorDrawable(Color.parseColor(colors[colors.length -1]));
        layers[1] = new ColorDrawable(Color.parseColor(colors[colors.length -1]));
        backgroundColorAnimation = new ChangeColor(layers);

        int sdk = android.os.Build.VERSION.SDK_INT;
        if(sdk < android.os.Build.VERSION_CODES.JELLY_BEAN) {
            this.setBackgroundDrawable(backgroundColorAnimation);
        } else {
            this.setBackground(backgroundColorAnimation);
        }

        Thread t = new Thread(this);
        t.start();
    }
    public LayoutColor(Activity context) {
        super(context);
        myView = this;
        this.setBackgroundColor(Color.WHITE);
    }

    @Override
    public void run() {

        try {
            int i = 0;
            while (true) {
                Thread.sleep(backgroundColorAnimation.getInterval());
                final String yoloColor = colors[i];
                context.runOnUiThread(new Runnable() {
                    public void run() {
                        backgroundColorAnimation.changeColor(Color.parseColor(yoloColor));
                        postInvalidate();
                    }
                });
                if(i!=colors.length -1)
                {
                    i++;
                }
                else
                {
                    i = 0;
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }
}
