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
uniform int materialShininess = 32;

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 v_normal;
in vec2 UV_Coord;
in vec3 position;
in vec3 vecToEye;

out vec4 FragColor;  

void main()
{
    // vec4 texel = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 texel = texture(texture1, UV_Coord); //texture smapling
    
    FragColor = materialAmbient * light_ambient * texel;

    vec3 normalizedNormal = normalize(v_normal);
       
    vec3 lightDirection = normalize(light_position - position);

    //calc distance for attenuation, ergo dampen the load
    float distance = length(light_position - position);
    float attenuation = 1.0 / (light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2.0));

    //diffuse / difIntensity FUCK DIF BAJEEEEEEEEEEEN
    float viHatarDIFIntensityBAJEEEN = max(dot(lightDirection, normalizedNormal), 0.0);

    //if fragment is facing light calc diffuse 
    if(viHatarDIFIntensityBAJEEEN > 0.0)
    {
        vec3 diffusePart = viHatarDIFIntensityBAJEEEN * light_diffuse.xyz * materialDiffuse.xyz * texel.xyz;
        FragColor.xyz += diffusePart * attenuation;

        vec3 vectorToEye = normalize(vecToEye);
        vec3 halfVector = normalize(lightDirection + vectorToEye);

        float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);
        float totalBrightness = initialBrightness; 
        
        for (int i = 0; i < materialShininess; i++) 
        {
            totalBrightness *= initialBrightness;
        }

        FragColor.xyz += totalBrightness * light_specular.xyz * materialSpecular.xyz * attenuation;
    }
}


    // //Ambient
    // float ambientStrength = 0.2;
    // vec3 ambient = ambientStrength * lightColor;
    //
    // //Diffuswe
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;
    //
    // //specular
    // float specularStrength = 0.5;
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 halfDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(norm, halfDir), 0.0), 32.0);
    // vec3 specular = specularStrength * spec * lightColor;
    //
    // //Combineee
    // vec3 result = (ambient + diffuse + specular) * ourColor;
    // //pre phong mix
    // vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    // FragColor = vec4(result, 1.0) * texColor;
