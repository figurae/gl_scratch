#version 330 core

out vec4 FragColor;
in vec4 aColOut;

uniform float time;

struct HslColor {
    float hue;
    float saturation;
    float lightness;
};

HslColor rgbToHsl(vec3 rgbColor) {
    float red = rgbColor.r, green = rgbColor.g, blue = rgbColor.b;

    float minChannel = min(min(red, green), blue);
    float maxChannel = max(max(red, green), blue);
    
    float hue, saturation = 0.0f;
    float lightness = (maxChannel + minChannel) / 2.0f;
    
    if (minChannel == maxChannel) return HslColor(hue, saturation, lightness);

    float deltaChannel = maxChannel - minChannel;
    saturation =
        lightness > 0.5f
            ? deltaChannel / (2.0f - maxChannel - minChannel)
            : deltaChannel / (maxChannel + minChannel);
    
    if (maxChannel == red)   hue = (green - blue) / deltaChannel + (green < blue ? 6.0f : 0.0f);
    if (maxChannel == green) hue = (blue - red)   / deltaChannel + 2.0f;
    if (maxChannel == blue)  hue = (red - green)  / deltaChannel + 4.0f;

    hue /= 6.0f;

    return HslColor(hue, saturation, lightness);
}

float hueToRgb(float lowerBound, float upperBound, float hue) {
    hue = mod(hue, 1.0);

    if (hue < 1.0f/6.0f) return lowerBound + (upperBound - lowerBound) * 6.0f * hue;
    if (hue < 1.0f/2.0f) return upperBound;
    if (hue < 2.0f/3.0f) return lowerBound + (upperBound - lowerBound) * (2.0f/3.0f - hue) * 6.0f;
    
    return lowerBound;
}

vec3 hslToRgb(HslColor hslColor) {
    float hue = hslColor.hue, saturation = hslColor.saturation, lightness = hslColor.lightness;
    float red, green, blue;

    if (saturation == 0) {
        red = green = blue = lightness;
    } else {
        float upperBound =
            lightness < 0.5f
                ? lightness * (1.0f + saturation)
                : lightness + saturation - lightness * saturation;
        float lowerBound = 2.0f * lightness - upperBound;

        red   = hueToRgb(lowerBound, upperBound, hue + 1.0f/3.0f);
        green = hueToRgb(lowerBound, upperBound, hue);
        blue  = hueToRgb(lowerBound, upperBound, hue - 1.0f/3.0f);
    }

    return vec3(red, green, blue);
}

vec4 animateColor(vec4 rgbColor) {
    HslColor hslColor = rgbToHsl(rgbColor.rgb);
    hslColor.hue += time;
    return vec4(hslToRgb(hslColor), rgbColor.a);
}

void main() {
    vec4 animatedColor = animateColor(aColOut);

    FragColor = vec4(animatedColor.r, animatedColor.g, animatedColor.b, animatedColor.a);
}
