//The shader for creating water splashes and reflections based on lighting in RenderMan
// (C) Dan Segal 2023

surface WaterSplash(
    float Kd = 0.8;         // Diffuse reflection coefficient
    float Ks = 0.2;         // Specular reflection coefficient
    float roughness = 0.1;  // Surface roughness
    color specularColor = 1;  // Specular reflection color
    float fresnelExp = 5;   // Fresnel exponent
    color waterColor = color(0.2, 0.4, 0.6);  // Water color
    float foamThreshold = 0.2;  // Foam threshold value
    float foamIntensity = 0.5;  // Foam intensity
    float foamSize = 0.05;      // Foam size
)
{
    normal Nf = faceforward(normalize(N), I);  // Surface normal
    
    // Compute reflected vector
    vector R = reflect(-I, Nf);
    
    // Compute reflection point position
    point P_reflection = P + R * 0.1;  // Reflection distance can be adjusted
    
    // Compute reflection color
    color reflectionColor = trace(P_reflection, R);
    
    // Compute specular color
    color specularColor = mix(specularColor, Cs, fresnel(Nf, I, fresnelExp));
    color specular = specularColor * reflectionColor;
    
    // Compute diffuse reflection
    color diffuse = Kd * Cs;
    
    // Compute foam
    float foam = smoothstep(foamThreshold - foamSize, foamThreshold + foamSize, random(P));
    color foamColor = foamIntensity * foam * Cs;
    
    // Compute final color
    color finalColor = diffuse + Ks * specular + foamColor;
    
    // Apply water color
    Ci = waterColor * finalColor;
    
    // Set alpha channel
    Oi = 1;
}
