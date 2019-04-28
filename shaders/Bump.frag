#version 330

uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

uniform vec4 u_color;

uniform sampler2D u_texture_3;
uniform vec2 u_texture_3_size;

uniform float u_normal_scaling;
uniform float u_height_scaling;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;
in vec2 v_uv;

out vec4 out_color;

float h(vec2 uv) {
    // You may want to use this helper function...
    vec4 map = texture(u_texture_3, uv);
    return map.r;
}

void main() {
    vec3 b = cross(v_normal.xyz, v_tangent.xyz);
    mat3 tbn = mat3(v_tangent.xyz, b, v_normal.xyz);

    vec2 xoff = vec2(v_uv.x + 1 / u_texture_3_size.x, v_uv.y);
    vec2 yoff = vec2(v_uv.x, v_uv.y + 1 / u_texture_3_size.y);
    float dU = (h(xoff) - h(v_uv)) * u_height_scaling * u_normal_scaling;
    float dV = (h(yoff) - h(v_uv)) * u_height_scaling * u_normal_scaling;
    vec3 no = vec3(-dU, -dV, 1);
    vec3 nd = tbn * no;
    vec4 nd_4d = vec4(nd.x, nd.y, nd.z, 1);

    
    vec3 ka = vec3(0.2, 0.2, 0.2);
    vec3 Ia =vec3 (1,1,1);
    vec3 kd = vec3(0.7, 0.7, 0.7);
    vec3 ks = vec3(0.5, 0.5, 0.5);
    float p = 8.0;

    // Light ray vector
    vec3 l = u_light_pos - v_position.xyz;
    vec3 l_dir = normalize(l);

    // Camera ray vector
    vec3 v = u_cam_pos - v_position.xyz;
    vec3 v_dir = normalize(v);

    // Bisector
    vec3 h = normalize(l_dir + v_dir);

    vec3 n_dir = normalize(nd.xyz);
    float r = length(l);

    vec3 out_color_3 =
    ka * Ia
    + kd * (u_light_intensity / pow(r, 2)) * max(0, dot(n_dir, l_dir))
    + ks * (u_light_intensity / pow(r, 2)) * pow(max(0, dot(n_dir, h)), p);
    out_color = vec4(out_color_3.x, out_color_3.y, out_color_3.z, 0);
    out_color.a = 1;
}

