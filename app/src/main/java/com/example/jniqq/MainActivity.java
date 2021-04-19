package com.example.jniqq;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Looper;
import android.view.View;
import android.widget.Toast;

import org.fmod.FMOD;

public class MainActivity extends AppCompatActivity {
    private static final int MODEL_NORMAL = 0;
    private static final int MODEL_LUOLI = 1;
    private static final int MODEL_DASHU = 2;
    private static final int MODEL_GAOGUAI = 3;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        FMOD.init(this);
        dynamicMethod();
        dynamicMethod1("aaa");
        nativeThread();
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close();
    }

    /**
     * 点击事件
     *
     * @param view
     */
    public void test(View view) {
        String path = "file:///android_asset/derry.mp3";
        voiceChange(MODEL_DASHU, path);
    }

    /**
     * 使用fmod对声音进行处理
     *
     * @param mode
     * @param path
     */
    private native void voiceChange(int mode, String path);//静态注册

//静态注册 最简单的方式 NDK开发方式是使用静态注册
//    缺点：
//    1、jni函数名非常长
//    2、捆绑上层包名和类名
//    3、运行效率慢,调用函数的时候才匹配

//    Android源码基本都是动态注册

    //动态注册
    public native void dynamicMethod();


    public native int dynamicMethod1(String name);

//    public native int dynamicMethod2(String value);

    //    线程
    public native void nativeThread();


    public void updateActivityUI() {
        if (Looper.getMainLooper() == Looper.myLooper()) {
            AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
            builder.setTitle("UI")
                    .setMessage("on UI Thread")
                    .setPositiveButton("yeah,I know", null)
                    .show();
        } else {
            runOnUiThread(() -> {
                Toast.makeText(this, "切换了", Toast.LENGTH_SHORT).show();
            });
        }
    }

}