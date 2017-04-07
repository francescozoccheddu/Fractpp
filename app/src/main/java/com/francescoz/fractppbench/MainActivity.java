package com.francescoz.fractppbench;

import android.app.Activity;
import android.os.Bundle;

import com.francescoz.fractpp.FPPConfig;
import com.francescoz.fractpp.FPPSurface;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        FPPSurface surface = new FPPSurface(this, new FPPConfig(), null);
        setContentView(surface.getView());
    }
}
