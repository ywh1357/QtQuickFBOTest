in highp vec2 texCoord;
uniform sampler2D Texture0;

void main(void)
{
    gl_FragColor = texture2D(Texture0, texCoord.st);
//    gl_FragColor = vec4(texCoord.x,texCoord.y,gl_FragCoord.z,1);
}
