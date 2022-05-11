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


Material default {
 0.588000;0.588000;0.588000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;
}

Frame root {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame FX_Lightning_Line_3_0 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh FX_Lightning_Line_3_0 {
   10;
   0.347435;-0.116762;0.270435;,
   0.218236;0.063202;0.053103;,
   0.303270;-0.093094;0.338086;,
   0.188809;0.103070;0.153902;,
   -0.084637;0.099999;-0.071369;,
   -0.125265;0.099477;0.003193;,
   -0.347435;0.110893;-0.274319;,
   -0.125265;0.099477;0.003193;,
   -0.311435;0.116762;-0.338086;,
   -0.084637;0.099999;-0.071369;;
   6;
   3;0,1,2;,
   3;3,2,1;,
   3;1,4,3;,
   3;5,3,4;,
   3;6,7,8;,
   3;9,8,7;;

   MeshMaterialList {
    1;
    6;
    0,
    0,
    0,
    0,
    0,
    0;
    { default }
   }

   DeclData {
    2;
    2;0;3;0;,
    1;0;5;0;;
    50;
    1058949342,
    992934768,
    1061761854,
    973283200,
    973280256,
    1059382785,
    1006240329,
    1061408458,
    1056957793,
    973280256,
    1058936764,
    993344577,
    1061771734,
    973283200,
    1065344827,
    1059295800,
    1004191477,
    1061481360,
    1056957793,
    1065344827,
    1057924671,
    1051660471,
    1061271252,
    1061149604,
    973280256,
    1058563025,
    1048566491,
    1061388574,
    1061149604,
    1065344827,
    1048698416,
    1061556914,
    1058315685,
    1065338007,
    1065344827,
    1058563025,
    1048566491,
    1061388574,
    1061149604,
    1065344827,
    1048674462,
    1061559423,
    1058317566,
    1065338007,
    973280256,
    1057924672,
    1051660471,
    1061271252,
    1061149604,
    973280256;
   }
  }
 }
}