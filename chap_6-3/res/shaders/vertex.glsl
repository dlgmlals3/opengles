uniform mat4 mvp_matrix;

attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute float aAngle;
attribute bool aChange;

varying vec2 vTexCoord;

vec4 rotateVertices(vec4 pos) {
    vec4 position = pos;
    position.x = position.x * cos(aAngle);
    position.z = position.z * sin(aAngle);
    return position;
}

vec4 rotateVerticesUp(vec4 pos) {
    vec4 position = pos;
    position.y = position.y * cos(aAngle);
    position.z = position.z * sin(aAngle);
    return position;
}

void main(void) {
    vTexCoord = aTexCoord;
    vec4 position = aPosition;

    if (aChange) {
        if (position.x > 0) {
            position = rotateVertices(position);
        }
    } else {
        if (position.y > 0) {
            position = rotateVerticesUp(position);
        }
    }

    gl_Position = mvp_matrix * position;
}
