#version 330 core

// lighting uniforms
uniform vec3 light_position = vec3(10, 25, 15);
uniform vec3 light_attenuation = vec3(1.0, 0.1, 0.01);

// light color 
uniform vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1.0);
uniform vec4 light_diffuse = vec4(1.0, 0.9, 0.8, 1.0);
uniform vec4 light_specular = vec4(0.9, 0.8, 0.7, 1.0);

// material properties
uniform vec4 materialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialSpecular = vec4(1.0, 1.0, 1.0, 1.0);
//uniform int materialShininess = 32;
//texture maps
//uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform sampler2D albedoMap;
uniform sampler2D specularMap;

in vec3 v_normal;
in vec2 UV_Coord;
in vec3 position;
in vec3 vecToEye;

out vec4 FragColor;  

void main()
{
    // vec4 texel = vec4(0.5, 0.5, 0.5, 1.0);
   // vec4 texel = texture(texture1, UV_Coord); //texture smapling
    vec4 albedoTexel = texture(albedoMap, UV_Coord);
    vec3 specularTexel = texture(specularMap, UV_Coord).rgb;    
    //
    // FragColor = materialAmbient * light_ambient * texel;
    FragColor = vec4(0.0, 0.0, 0.0, albedoTexel.w);

    vec3 normalizedNormal = normalize(v_normal);  
    vec3 lightDirection = normalize(light_position - position);
    
    //ambient    
    FragColor.xyz += albedoTexel.xyz * materialAmbient.xyz * light_ambient.xyz;

    //calc distance for attenuation, ergo dampen the load
    float distance = length(light_position - position);
    float attenuation = 1.0 / (light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2.0));

    //diffuse / difIntensity FUCK DIF BAJEEEEEEEEEEEN
    float viHatarDIFIntensityBAJEEEN = max(dot(lightDirection, normalizedNormal), 0.0);

    //if fragment is facing light calc diffuse 
    if(viHatarDIFIntensityBAJEEEN > 0.0)
    {
        FragColor.xyz += albedoTexel.xyz * viHatarDIFIntensityBAJEEEN * light_diffuse.xyz * attenuation;
        
        vec3 vectorToEye = normalize(vecToEye);
        vec3 halfVector = normalize(lightDirection + vectorToEye);
        float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);

        float shininess = max(specularTexel.r * 128.0, 1.0);
        float totalBrightness = pow(initialBrightness, shininess);

        FragColor.xyz += totalBrightness * light_specular.xyz * materialSpecular.xyz * attenuation;
    }
}
