package daesolus.RemoteColor;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.TransitionDrawable;
/**
 * Created by DaeSoluS on 2015-04-07.
 */
public class ChangeColor  extends TransitionDrawable  {
    private int interval;
    public ChangeColor(Drawable[] layers) {
        super(layers);
        interval = 2000;
        initVars();
    }
    public ChangeColor(Drawable[] layers, int interval) {
        super(layers);
        this.interval = interval;
        initVars();
    }
    private void initVars(){
        setCrossFadeEnabled(true);
        setId(0,0);
        setId(1,1);
    }
    public void changeColor(int color){
        setDrawableByLayerId(0, getDrawable(1));
        setDrawableByLayerId(1, new ColorDrawable(color));
        startTransition(interval);
    }
    public void changeColor(String color){
        setDrawableByLayerId(0, getDrawable(1));
        setDrawableByLayerId(1, new ColorDrawable(Color.parseColor(color)));
        startTransition(interval);
    }

    public int getInterval()
    {
        return this.interval;
    }
}
