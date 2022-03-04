#define highp

uniform sampler2D uTexture;
varying highp vec2 vTexCoord;

void main(void)
{
    gl_FragColor = texture2D(uTexture, vTexCoord);
}
