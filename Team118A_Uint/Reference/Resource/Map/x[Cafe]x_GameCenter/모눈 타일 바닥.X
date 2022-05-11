xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template VertexElement {
 <f752461c-1e23-48f6-b9f8-8350850f336f>
 DWORD Type;
 DWORD Method;
 DWORD Usage;
 DWORD UsageIndex;
}

template DeclData {
 <bf22e553-292c-4781-9fea-62bd554bdd93>
 DWORD nElements;
 array VertexElement Elements[nElements];
 DWORD nDWords;
 array DWORD data[nDWords];
}


Material Material__86 {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "my_game_01_p_tile_01.png";
 }
}

Frame root {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame my_gamecenter_01_p_tile_01_0 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,-0.000000,1.000000,0.000000,0.000000,-1.000000,-0.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh my_gamecenter_01_p_tile_01_0 {
   12;
   -0.300000;0.300000;0.284347;,
   -0.300000;-0.300000;0.000000;,
   -0.300000;-0.300000;0.284347;,
   -0.300000;0.300000;0.000000;,
   0.300000;0.300000;0.000000;,
   -0.300000;-0.300000;0.000000;,
   -0.300000;0.300000;0.000000;,
   0.300000;-0.300000;0.000000;,
   0.300000;-0.300000;0.284347;,
   -0.300000;-0.300000;0.000000;,
   0.300000;-0.300000;0.000000;,
   -0.300000;-0.300000;0.284347;;
   6;
   3;0,1,2;,
   3;3,1,0;,
   3;4,5,6;,
   3;7,5,4;,
   3;8,9,10;,
   3;11,9,8;;

   MeshMaterialList {
    1;
    6;
    0,
    0,
    0,
    0,
    0,
    0;
    { Material__86 }
   }

   DeclData {
    2;
    2;0;3;0;,
    1;0;5;0;;
    60;
    3212836864,
    0,
    0,
    1010560020,
    1015859200,
    3212836864,
    0,
    0,
    1047999186,
    1057004360,
    3212836864,
    0,
    0,
    1010560708,
    1057004366,
    3212836864,
    0,
    0,
    1047999172,
    1015858912,
    0,
    0,
    3212836864,
    1065167756,
    1048833224,
    0,
    0,
    3212836864,
    1048701510,
    1065177416,
    0,
    0,
    3212836864,
    1048701504,
    1048833224,
    0,
    0,
    3212836864,
    1065167756,
    1065177422,
    0,
    3212836864,
    0,
    1010717989,
    1065147599,
    0,
    3212836864,
    0,
    1048009064,
    1057007554,
    0,
    3212836864,
    0,
    1048009051,
    1065147607,
    0,
    3212836864,
    0,
    1010718698,
    1057007550;
   }
  }
 }
}