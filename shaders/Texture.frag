//#version 330
//
//uniform vec3 u_cam_pos;
//uniform vec3 u_light_pos;
//uniform vec3 u_light_intensity;
//
//uniform sampler2D u_texture_earth;
//uniform sampler2D u_texture_jupiter;
//uniform sampler2D u_texture_mars;
//uniform sampler2D u_texture_sun;
////
//uniform float u_normal_scaling;
//uniform float u_height_scaling;
//
//uniform int type;
//
//in vec4 v_position;
//in vec4 v_normal;
//in vec4 v_tangent;
//in vec2 v_uv;
//
//out vec4 out_color;
//const vec3 flat_normal = vec3(0, 0, 1);
//
//void main() {
//  // YOUR CODE HERE
//    vec4 s;
//    if (type == 0) {
//        s = texture(u_texture_universe, v_uv);
//        out_color = vec4(s.x, s.y, s.z, 1);
//    } else if (type == 4){
//        s = texture(u_texture_sun, v_uv);
//        out_color = vec4(s.x, s.y, s.z, 1);
//    } else {
//        if (type == 1) {
//            s = texture(u_texture_earth, v_uv);
//        } else if (type == 2) {
//            s = texture(u_texture_jupiter, v_uv);
//        } else if (type == 3) {
//            s = texture(u_texture_mars, v_uv);
//        } else if (type == 5) {
//            s = texture(u_texture_mercury, v_uv);
//        } else if (type == 6) {
//            s = texture(u_texture_moon, v_uv);
//        } else if (type == 7) {
//            s = texture(u_texture_neptune, v_uv);
//        } else if (type == 8) {
//            s = texture(u_texture_moon, v_uv);
//        } else if (type == 9) {
//            s = texture(u_texture_venus, v_uv);
//        } else if (type == 10) {
//            s = texture(u_texture_uranus, v_uv);
//        }
//        else if (type == 11) {
//            s = texture(u_texture_saturn, v_uv);
//        }
//        vec3 ka = vec3(0.6, 0.6, 0.6);
//        vec3 Ia = s.xyz;
//        vec3 kd = vec3(0.5, 0.5, 0.5);
//        vec3 ks = vec3(0.8, 0.8, 0.8);
//        float p = 8.0;
//
//        // Light ray vector
//        vec3 l = u_light_pos - v_position.xyz;
//        vec3 l_dir = normalize(l);
//
//        // Camera ray vector
//        vec3 v = u_cam_pos - v_position.xyz;
//        vec3 v_dir = normalize(v);
//
//        // Bisector
//        vec3 h = normalize(l_dir + v_dir);
//
//        vec3 n_dir = normalize(v_normal.xyz);
//        float r = length(l);
//
//        vec3 out_color_3 =ka * Ia + kd * (u_light_intensity / pow(r, 1)) * max(0, dot(n_dir, l_dir)) + ks * (u_light_intensity / pow(r, 4)) * pow(max(0, dot(n_dir, h)), p);
//    }
//
//
//}
//

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
uniform vec2 u_texture_earth_bump;

uniform float u_normal_scaling;
uniform float u_height_scaling;

uniform int type;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;
in vec2 v_uv;

out vec4 out_color;
const vec3 flat_normal = vec3(0, 0, 1);

//float h(vec2 uv) {
//    vec4 map = texture(u_texture_earth, uv);
//    return map.r;
//}

void main() {
    // YOUR CODE HERE
    vec4 s;
    if (type == 0) {
        s = texture(u_texture_universe, v_uv);
        out_color = vec4(s.x, s.y, s.z, 1);
    } else if (type == 4){
        s = texture(u_texture_sun, v_uv);
        out_color = vec4(s.x, s.y, s.z, 1);
    } else {
        if (type == 1) {
            s = texture(u_texture_earth, v_uv);
        } else if (type == 2) {
            s = texture(u_texture_jupiter, v_uv);
        } else if (type == 3) {
            s = texture(u_texture_mars, v_uv);
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
        } else if (type == 11) {
            s = texture(u_texture_saturn, v_uv);
        }
        vec3 ka = vec3(0.6, 0.6, 0.6);
        vec3 Ia = s.xyz;
        vec3 kd = vec3(0.5, 0.5, 0.5);
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
        
        vec3 out_color_3 =ka * Ia + kd * (u_light_intensity / pow(r, 1)) * max(0, dot(n_dir, l_dir)) + ks * (u_light_intensity / pow(r, 4)) * pow(max(0, dot(n_dir, h)), p);
        out_color = vec4(out_color_3.x, out_color_3.y, out_color_3.z, 1);
    }
    
}
