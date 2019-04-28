#version 330

uniform vec4 u_color;
uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_uv;

out vec4 out_color;

void main() {
  // YOUR CODE HERE
    vec3 ka = vec3(0.2, 0.2, 0.2);
    vec3 Ia = u_color.xyz;
    //vec3(0.6, 0.8, 0.9);
    vec3 kd = vec3(0.7, 0.7, 0.7);
    vec3 ks = vec3(0.8, 0.8, 0.8);
    float p = 8.0;
    
    // Light ray vector
    vec3 l = u_light_pos - v_position.xyz;
    vec3 l_dir = normalize(l);
    
    // Camera ray vector
    vec3 v = u_cam_pos - v_position.xyz;
    vec3 v_dir = normalize(v);
    
    // Bisector
    vec3 h = normalize(l_dir + v_dir);
    
    vec3 n_dir = normalize(v_normal.xyz);
    float r = length(l);

    vec3 out_color_3 =
                  ka * Ia
                + kd * (u_light_intensity / pow(r, 2)) * max(0, dot(n_dir, l_dir))
                + ks * (u_light_intensity / pow(r, 2)) * pow(max(0, dot(n_dir, h)), p);
    out_color = vec4(out_color_3.x, out_color_3.y, out_color_3.z, 0);
    out_color.a = 1;
}

