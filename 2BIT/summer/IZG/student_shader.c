/*!
 * @file 
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include<math.h>
#include<assert.h>

#include"student/student_shader.h"
#include"student/gpu.h"
#include"student/uniforms.h"

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(
    GPUVertexShaderOutput     *const output,
    GPUVertexShaderInput const*const input ,
    GPU                        const gpu   ){
  /// \todo Naimplementujte vertex shader, který transformuje vstupní vrcholy do clip-space.<br>
  /// <b>Vstupy:</b><br>
  /// Vstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu ve world-space (vec3) a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).<br>
  /// <b>Výstupy:</b><br>
  /// Výstupní vrchol by měl v nultém atributu obsahovat pozici vrcholu (vec3) ve world-space a v prvním
  /// atributu obsahovat normálu vrcholu ve world-space (vec3).
  /// Výstupní vrchol obsahuje pozici a normálu vrcholu proto, že chceme počítat osvětlení ve world-space ve fragment shaderu.<br>
  /// <b>Uniformy:</b><br>
  /// Vertex shader by měl pro transformaci využít uniformní proměnné obsahující view a projekční matici.
  /// View matici čtěte z uniformní proměnné "viewMatrix" a projekční matici čtěte z uniformní proměnné "projectionMatrix".
  /// Zachovejte jména uniformních proměnných a pozice vstupních a výstupních atributů.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Využijte vektorové a maticové funkce.
  /// Nepředávajte si data do shaderu pomocí globálních proměnných.
  /// Pro získání dat atributů použijte příslušné funkce vs_interpret* definované v souboru program.h.
  /// Pro získání dat uniformních proměnných použijte příslušné funkce shader_interpretUniform* definované v souboru program.h.
  /// Vrchol v clip-space by měl být zapsán do proměnné gl_Position ve výstupní struktuře.<br>
  /// <b>Seznam funkcí, které jistě použijete</b>:
  ///  - gpu_getUniformsHandle()
  ///  - getUniformLocation()
  ///  - shader_interpretUniformAsMat4()
  ///  - vs_interpretInputVertexAttributeAsVec3()
  ///  - vs_interpretOutputVertexAttributeAsVec3()
  //(void)output;
  //(void)input;
  //(void)gpu;



  Uniforms const Handler = gpu_getUniformsHandle(gpu);

  Mat4 const*const view = shader_interpretUniformAsMat4(Handler, getUniformLocation(gpu, "viewMatrix"));
  Mat4 const*const proj = shader_interpretUniformAsMat4(Handler, getUniformLocation(gpu, "projectionMatrix"));
  Vec3 const*const pos = vs_interpretInputVertexAttributeAsVec3(gpu, input, 0);
  Vec3 const*const nor = vs_interpretInputVertexAttributeAsVec3(gpu, input, 1);
  Mat4 move;
  Vec4 posfor;

  multiply_Mat4_Mat4(&move,proj,view);
  copy_Vec3Float_To_Vec4(&posfor,pos,1.f);
  multiply_Mat4_Vec4(&output->gl_Position, &move, &posfor);

  Vec3 *const oposite_pos = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 0);
  Vec3 *const oposite_nor = vs_interpretOutputVertexAttributeAsVec3(gpu, output, 1);
  copy_Vec3(oposite_pos, pos);
  copy_Vec3(oposite_nor, nor);

}

void phong_fragmentShader(
    GPUFragmentShaderOutput     *const output,
    GPUFragmentShaderInput const*const input ,
    GPU                          const gpu   ){
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br> 
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte na čistou zelenou.
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()
  //(void)output;
  //(void)input;
  //(void)gpu;

  Vec3 const*const pos = fs_interpretInputAttributeAsVec3(gpu, input, 0);
  Vec3 const*const nor = fs_interpretInputAttributeAsVec3(gpu, input, 1);

  Uniforms const Handler = gpu_getUniformsHandle(gpu);
  UniformLocation const cameraPosition = getUniformLocation(gpu, "cameraPosition");
  UniformLocation const lightPosition = getUniformLocation(gpu, "lightPosition");
  Vec3 const*const view = shader_interpretUniformAsVec3(Handler, cameraPosition);
  Vec3 const*const proj = shader_interpretUniformAsVec3(Handler, lightPosition);

  Vec3 material_dif,material_clean,shines,sun,normalized_nor,normalized_lig,normalized_view,normalized_lightt,right,color_fin;
  init_Vec3(&material_dif, 0.f,1.f,0.f);
  init_Vec3(&material_clean, 1.f,1.f,1.f);
  init_Vec3(&shines, 1.f,1.f,1.f);
  init_Vec3(&sun, 1.f,1.f,1.f);

  normalize_Vec3(&normalized_nor, nor);
  sub_Vec3(&normalized_lig, proj, pos);
  normalize_Vec3(&normalized_lig, &normalized_lig);
  sub_Vec3(&normalized_view, view, pos);
  normalize_Vec3(&normalized_view, &normalized_view);

  float lf = dot_Vec3(&normalized_lig, &normalized_nor);
    if (lf < 0.f)
      lf = 0.f;
    if (lf > 1.f)
      lf = 1.f;

  multiply_Vec3_Float(&normalized_lightt, &normalized_lig, -1);
  reflect(&right, &normalized_lightt,&normalized_nor);
  
  float rl = dot_Vec3(&right, &normalized_view);
    if (rl < 0.f)
      rl = 0.f;
    if (rl > 1.f)
      rl = 1.f;

  multiply_Vec3_Float(&material_dif, &material_dif, lf);
  multiply_Vec3_Float(&sun, &sun, (float)pow(rl,40.f));

  add_Vec3(&color_fin, &material_dif, &sun);

  copy_Vec3Float_To_Vec4(&output->color, &color_fin, 1.f);  

}

/// @}
