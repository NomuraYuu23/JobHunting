float32_t Gauss(in const float32_t i, in const float32_t sigma) {

	return rcp(2.0f * PI * sigma * sigma) * exp(-(i * i) * rcp(2.0f * sigma * sigma));

}

float32_t Noise(in const float32_t2 coord)
{

	return frac(sin(dot(coord, float32_t2(8.7819f, 3.255f))) * 437.645);

}

float32_t2 GetTexcoord(in const float32_t2 index, in const float32_t2 textureSize) {

	return float32_t2(
		index.x * rcp(textureSize.x),
		index.y * rcp(textureSize.y));

}

float32_t Luminance(float32_t3 v) {
	return dot(v, float32_t3(0.2125f, 0.7154f, 0.0721f));
}

float32_t3 RGBToHSV(in const float32_t3 rgb) {

	float32_t3 hsv = { 0.0f,0.0f,0.0f };

	// RGBの最小値、最大値を求める
	float32_t min = rgb.r;
	float32_t max = rgb.r;

	if (min > rgb.g) {
		min = rgb.g;
	}
	if (min > rgb.b) {
		min = rgb.b;
	}

	if (max < rgb.g) {
		max = rgb.g;
	}
	if (max < rgb.b) {
		max = rgb.b;
	}

	// hを求める
	hsv.x = max - min;
	if (hsv.x > 0.0f) {
		if (max == r) {
			hsv.x = (rgb.g - rgb.b) / hsv.x;
			if (hsv.x < 0.0f) {
				hsv.x += 6.0f;
			}
		}
		else if (max == rgb.g) {
			hsv.x = 2.0f + (rgb.b - rgb.r) / hsv.x;
		}
		else {
			hsv.x = 4.0f + (rgb.r - rgb.g) / hsv.x;
		}
	}
	hsv.x /= 6.0f;

	// sを求める
	hsv.y = (max - min);
	if (max != 0.0f) {
		hsv.y /= max;
	}

	// vを求める
	hsv.z = max;

	return hsv;

}

float32_t3 HSVToRGB(in const float32_t3 hsv) {

	float32_t3 rgb;

	rgb.r = hsv.v;
	rgb.g = hsv.v;
	rgb.b = hsv.v;

	if (hsv.y > 0.0f) {
		float32_t h = hsv.x * 6.0f;
		int32_t i = int32_t(h);
		float32_t f = h - float32_t(i);

		switch (i) {
		default:
		case 0:
			rgb.g *= 1.0f - s * (1.0f - f);
			rgb.b *= 1.0f - s;
			break;
		case 1:
			rgb.r *= 1.0f - s * f;
			rgb.b *= 1.0f - s;
			break;
		case 2:
			rgb.r *= 1.0f - s;
			rgb.b *= 1.0f - s * (1.0f - f);
			break;
		case 3:
			rgb.r *= 1.0f - s;
			rgb.g *= 1.0f - s * f;
			break;
		case 4:
			rgb.r *= 1.0f - s * (1.0f - f);
			rgb.g *= 1.0f - s;
			break;
		case 5:
			rgb.g *= 1.0f - s;
			rgb.b *= 1.0f - s * f;
			break;
		}
	}

	return rgb;

}
