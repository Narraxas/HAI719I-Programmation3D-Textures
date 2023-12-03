#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in mat3 TBN;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform samplerCube skybox;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 colorFromTexture = texture(colorTexture, o_uv0).rgb;
    vec3 normalFromTexture = texture(normalTexture, o_uv0).rgb;
    normalFromTexture = normalize(normalFromTexture * 2.0 - 1.0); // Ramener la normal map dans l'espace tangeant

    vec3 lightColor = vec3(1,1,1);

    // --------------------- Phong classique ---------------------
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor ;
    // diffuse
    vec3 norm = normalize ( TBN * normalFromTexture ) ;
    vec3 lightDir = normalize ( lightPos - o_positionWorld ) ;
    float diff = max ( dot ( norm , lightDir ) , 0.0) ; // sat(n.l)
    vec3 diffuse = diff * lightColor;
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize ( viewPos - o_positionWorld ) ;
    vec3 reflectDir = reflect ( - lightDir , norm ) ;
    float spec = pow ( max ( dot ( viewDir , reflectDir ) , 0.0) , 32) ;
    vec3 specular = specularStrength * spec * lightColor ;
    vec4 result = vec4 ( ( ambient + diffuse + specular ) , 1.0);
    FragColor = result * texture(colorTexture, o_uv0);

    // ---------------------   Réflexions   ----------------------

    vec3 reflectedView = reflect(-viewDir, normalize(o_normalWorld));
    vec4 reflectionColor = vec4(texture(skybox, reflectedView).rgb, 1.0);
    // FragColor = reflectionColor; // Décommenter pour ajouter le rendu réflectif

    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    // FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);
}
