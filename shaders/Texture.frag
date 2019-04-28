#version 330

uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

uniform sampler2D u_texture_4;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_uv;

out vec4 out_color;

void main() {
  // YOUR CODE HERE
    vec4 s = texture(u_texture_4, v_uv);
  // (Placeholder code. You will want to replace it.)
    out_color = vec4(s.x, s.y, s.z, 0);
    out_color.a = 1;
}
