#version 330 core
#define MAX_LIGHTS 16

uniform vec4 global_ambient = vec4(0.2, 0.2, 0.2, 1.0);

// lighting uniforms
uniform int activeLights = 1;
uniform int light_type[MAX_LIGHTS];
uniform vec3 light_position[MAX_LIGHTS];
uniform vec3 light_direction[MAX_LIGHTS];

// light color 
uniform vec4 light_diffuse[MAX_LIGHTS];
uniform vec4 light_specular[MAX_LIGHTS];
uniform vec3 light_attenuation[MAX_LIGHTS];
uniform float light_spotCutoff[MAX_LIGHTS];
uniform int light_spotExponent[MAX_LIGHTS];

// material properties
uniform vec4 materialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialSpecular = vec4(1.0, 1.0, 1.0, 1.0);

//shadow mapping
uniform sampler2D shadowMap;
uniform mat4 shadowMapMatrix;
uniform vec2 shadowMapTexelSize = vec2(1.0/2048.0, 1.0/2048.0);
uniform float shadowCalcBias = 0.0005;
uniform bool bUseShadowMap = false;
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

float CalcShadow()
{
     if (!bUseShadowMap)
        return 0.0;
    
    vec4 shadowSpace = shadowMapMatrix * vec4(position, 1.0);
   
    shadowSpace.xyz /= shadowSpace.w;
    
    // check if coordinates are within shadow map range [0, 1]
    if (shadowSpace.xy.x < 0.0 || shadowSpace.xy.x > 1.0 ||
        shadowSpace.xy.y < 0.0 || shadowSpace.xy.y > 1.0)
    {
        return 0.0;
    }
    
    // sample shadow map
    float closestDepth = texture(shadowMap, shadowSpace.xy).r;
    float fragDepth = shadowSpace.z;
    
    if (fragDepth > closestDepth + shadowCalcBias)
    {
        return 1.0;
    }
    
    return 0.0;
}

void main()
{
    // vec4 texel = vec4(0.5, 0.5, 0.5, 1.0);
   // vec4 texel = texture(texture1, UV_Coord); //texture smapling
    vec4 albedoTexel = texture(albedoMap, UV_Coord);
    vec3 specularTexel = texture(specularMap, UV_Coord).rgb;    
    //
    // FragColor = materialAmbient * light_ambient * texel;
    vec3 normalizedNormal = normalize(v_normal);
    vec3 viewDir = normalize(vecToEye);

    FragColor = vec4(0.0, 0.0, 0.0, albedoTexel.a);

    //vec3 lightDirection = normalize(light_position - position);
    
    //ambient    
    FragColor.xyz += albedoTexel.xyz * materialAmbient.xyz * global_ambient.xyz;

    for (int i = 0; i < activeLights; i++)
    {
        vec3 lightDirection = vec3(0.0);
        float attenuation = 1.0;

      //loop active lights
    if (light_type[i] == 0)
        {
            // point light
            vec3 LightPos = light_position[i] - position;
            float distance = length(LightPos);
            lightDirection = normalize(LightPos);
            attenuation = 1.0 / (light_attenuation[i].x + light_attenuation[i].y * distance + light_attenuation[i].z * distance * distance);
        }
        else if (light_type[i] == 1)
        {
            // directional light
            lightDirection = normalize(-light_direction[i]);
            attenuation = 1.0;
        }
        else if (light_type[i] == 2)
        {
            // spotlight
            vec3 LightPos = light_position[i] - position;
            float distance = length(LightPos);
            lightDirection = normalize(LightPos);
            attenuation = 1.0 / (light_attenuation[i].x + light_attenuation[i].y * distance + light_attenuation[i].z * distance * distance);

            float spotDot = dot(normalize(-light_direction[i]), lightDirection);
            if (spotDot < light_spotCutoff[i])
            {
                attenuation = 0.0;
            }
            else
            {
                attenuation *= pow(spotDot, float(light_spotExponent[i]));
            }
        }
        if (attenuation <= 0.0)
            continue;
        //diffuse / difIntensity FUCK DIF BAJEEEEEEEEEEEN
        float viHatarDIFIntensityBAJEEEN = max(dot(lightDirection, normalizedNormal), 0.0);

         if(viHatarDIFIntensityBAJEEEN > 0.0 && attenuation > 0.0)
        {
            float shadow = CalcShadow();
            FragColor.xyz += (1.0 - shadow) * albedoTexel.xyz * viHatarDIFIntensityBAJEEEN * light_diffuse[i].xyz * attenuation;

            vec3 vectorToEye = normalize(vecToEye);
            vec3 halfVector = normalize(lightDirection + vectorToEye);
            float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);

            float shininess = max(specularTexel.r * 128.0, 1.0);
            float totalBrightness = pow(initialBrightness, shininess);

            FragColor.xyz += (1.0 - shadow) * totalBrightness * light_specular[i].xyz * materialSpecular.xyz * attenuation;
        }
    }
}
    //calc distance for attenuation, ergo dampen the load
    // float distance = length(light_position - position);
    // float attenuation = 1.0 / (light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2.0));


    //if fragment is facing light calc diffuse 
   
