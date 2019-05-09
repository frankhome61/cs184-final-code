#version 330

uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

uniform sampler2D u_texture_earth;
uniform sampler2D u_texture_jupiter;
uniform sampler2D u_texture_mars;
uniform sampler2D u_texture_sun;

uniform sampler2D u_texture_mercury;
uniform sampler2D u_texture_moon;
uniform sampler2D u_texture_neptune;
uniform sampler2D u_texture_venus;
uniform sampler2D u_texture_uranus;
uniform sampler2D u_texture_saturn;


uniform sampler2D u_texture_universe;

uniform int type;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_uv;

out vec4 out_color;

void main() {
  // YOUR CODE HERE
    vec4 s;
    if (type == 0) {
        s = texture(u_texture_universe, v_uv);
    } else if (type == 1) {
        s = texture(u_texture_earth, v_uv);
    } else if (type == 2) {
        s = texture(u_texture_jupiter, v_uv);
    } else if (type == 3) {
        s = texture(u_texture_mars, v_uv);
    } else if (type == 4) {
        s = texture(u_texture_sun, v_uv);
    } else if (type == 5) {
        s = texture(u_texture_mercury, v_uv);
    } else if (type == 6) {
        s = texture(u_texture_moon, v_uv);
    } else if (type == 7) {
        s = texture(u_texture_neptune, v_uv);
    } else if (type == 8) {
        s = texture(u_texture_moon, v_uv);
    } else if (type == 9) {
        s = texture(u_texture_venus, v_uv);
    } else if (type == 10) {
        s = texture(u_texture_uranus, v_uv);
    }
    else if (type == 11) {
        s = texture(u_texture_saturn, v_uv);
    }
      // (Placeholder code. You will want to replace it.)
        out_color = vec4(s.x, s.y, s.z, 0);
        out_color.a = 1;
    
}
