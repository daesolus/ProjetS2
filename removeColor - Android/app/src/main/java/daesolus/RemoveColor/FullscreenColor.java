package daesolus.RemoveColor;

import android.os.Bundle;
import android.app.Activity;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

import com.koushikdutta.async.http.AsyncHttpClient;
import com.koushikdutta.async.http.WebSocket;
import static junit.framework.Assert.assertEquals;


public class FullscreenColor extends Activity {

    private FullscreenColor frame;
    private int test = 0;
    private String colors[];
    private float x1 ,x2,y1,y2;
    private WebSocket ws;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        frame = this;

        setContentView(new LayoutColor(frame));

        WindowManager.LayoutParams layoutParam = getWindow().getAttributes();
        layoutParam.screenBrightness = 1F;
        getWindow().setAttributes(layoutParam);

        AsyncHttpClient.getDefaultInstance().websocket("http://107.170.171.251:56453", null,
                new AsyncHttpClient.WebSocketConnectCallback() {
            @Override
            public void onCompleted(Exception ex,  WebSocket webSocket) {
                ws = webSocket;
                if (ex != null) {
                    ex.printStackTrace();
                    return;
                }
                webSocket.setStringCallback(new WebSocket.StringCallback() {
                    @Override
                    public void onStringAvailable(String s) {
                      if(s.charAt(0) == '#') {
                          colors = s.split(",");
                          runOnUiThread(new Runnable() {
                              @Override
                              public void run() {
                                  setContentView(new LayoutColor(frame, colors));
                                  ;
                              }
                          });
                      }

                    }
                });
            }
        });
    }
    @Override
    public boolean onTouchEvent(MotionEvent touchevent) {
        switch (touchevent.getAction()) {
            case MotionEvent.ACTION_DOWN: {
                x1 = touchevent.getX();
                y1 = touchevent.getY();
                break;
            }
            case MotionEvent.ACTION_UP: {
                x2 = touchevent.getX();
                y2 = touchevent.getY();
                //if left to right sweep event on screen
                if (x1 < x2 && (x2 - x1)> 200) {
                    ws.send("!r");
                }
                // if right to left sweep event on screen
                else if (x1 > x2 && (x1 - x2)> 200) {
                    ws.send("!l");
                }
                else {
                    ws.send("!m");
                }
                break;
            }
        }
        return false;
    }
}