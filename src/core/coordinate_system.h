class CoordinateCalculator {
    public:
    static int referenceWidth;
    static int referenceHeight;
    static float meterPixelRatioX;
    static float meterPixelRatioY;

    static void ConvertMetersToPixels(float& xMeter, float& yMeter);

    static void ConvertPixelsToMeters(float& xPixel, float& yPixel);

    static void NormalizePixelCoord(float& xPixel, float& yPixel);
};
