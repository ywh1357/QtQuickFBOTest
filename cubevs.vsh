attribute highp vec3 Vertex;
attribute highp vec2 aTexCoord;
out highp vec2 texCoord;
uniform highp mat4 mvp;

void main(void)
{
    texCoord = vec2(aTexCoord.x,aTexCoord.y);
    gl_Position = mvp * vec4(Vertex,1.0);
}
