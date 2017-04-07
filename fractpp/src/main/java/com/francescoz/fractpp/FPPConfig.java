package com.francescoz.fractpp;

public class FPPConfig {

    public ColorPrecision colorPrecision = ColorPrecision.HIGH;
    public AlphaPrecision alphaPrecision = AlphaPrecision.NONE;
    public DepthPrecision depthPrecision = DepthPrecision.NONE;
    public AAPrecision aaPrecision = AAPrecision.NONE;

    public enum ColorPrecision {
        LOW(4, 4, 4), MID(5, 6, 5), HIGH(8, 8, 8);

        public final int bitsR, bitsG, bitsB;

        ColorPrecision(int bitsR, int bitsG, int bitsB) {
            this.bitsR = bitsR;
            this.bitsG = bitsG;
            this.bitsB = bitsB;
        }
    }

    public enum AlphaPrecision {
        NONE(0), LOW(4), MEDIUM(8), HIGH(16);

        public final int bits;

        AlphaPrecision(int bits) {
            this.bits = bits;
        }

        public boolean isTranslucent() {
            return bits > 0;
        }

    }

    public enum DepthPrecision {
        NONE(0), LOW(4), MEDIUM(8), HIGH(16);

        public final int bits;

        DepthPrecision(int bits) {
            this.bits = bits;
        }

    }

    public enum AAPrecision {
        NONE(0), LOW(2), MEDIUM(4), HIGH(8);

        public final int samples;

        AAPrecision(int samples) {
            this.samples = samples;
        }
    }

}
