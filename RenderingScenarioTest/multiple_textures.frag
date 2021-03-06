#version 120

in vec2 UV;
in vec3 pos;

uniform int texCount;
uniform int texCountX;
uniform int texCountY;

uniform sampler2D texture0[16];
uniform sampler2D texture1[16];
uniform sampler2D texture2[16];
uniform sampler2D texture3[16];
uniform sampler2D texture4[16];
uniform sampler2D texture5[16];
uniform sampler2D texture6[16];
uniform sampler2D texture7[16];
uniform sampler2D texture8[16];
uniform sampler2D texture9[16];
uniform sampler2D texture10[16];
uniform sampler2D texture11[16];
uniform sampler2D texture12[16];
uniform sampler2D texture13[16];
uniform sampler2D texture14[16];
uniform sampler2D texture15[16];

void main(void)
{
    float unitLengthX = 1.0 / float(texCountX);
    float unitLengthY = 1.0 / float(texCountY);
    int texIndexX = int(UV.x / unitLengthX);
    int texIndexY = int(UV.y / unitLengthY);
    int texIndex = texIndexY * texCountX + texIndexX;
    float refX = UV.x * texCountX - texIndexX;
    float refY = UV.y * texCountY - texIndexY;

    int groupIdx = texIndex / 16;

    switch(groupIdx)
    {
    case 0:
        gl_FragColor = texture2D(texture0[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 1:
        gl_FragColor = texture2D(texture1[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 2:
        gl_FragColor = texture2D(texture2[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 3:
        gl_FragColor = texture2D(texture3[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 4:
        gl_FragColor = texture2D(texture4[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 5:
        gl_FragColor = texture2D(texture5[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 6:
        gl_FragColor = texture2D(texture6[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 7:
        gl_FragColor = texture2D(texture7[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 8:
        gl_FragColor = texture2D(texture8[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 9:
        gl_FragColor = texture2D(texture9[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 10:
        gl_FragColor = texture2D(texture10[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 11:
        gl_FragColor = texture2D(texture11[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 12:
        gl_FragColor = texture2D(texture12[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 13:
        gl_FragColor = texture2D(texture13[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 14:
        gl_FragColor = texture2D(texture14[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    case 15:
        gl_FragColor = texture2D(texture15[texIndex - groupIdx*16], vec2(refX, -refY));
        break;
    }
}
