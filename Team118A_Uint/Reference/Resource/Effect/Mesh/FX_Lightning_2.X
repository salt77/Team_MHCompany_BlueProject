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

 Frame FX_Lightning_Line_2_0 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh FX_Lightning_Line_2_0 {
   8;
   0.276883;-0.145835;0.327177;,
   0.350544;0.071554;0.115450;,
   0.233034;-0.138210;0.340031;,
   0.297966;0.099226;0.113647;,
   0.126942;0.089147;-0.118980;,
   0.051354;0.088625;-0.099433;,
   -0.279324;0.145835;-0.340030;,
   -0.350544;0.139966;-0.275790;;
   6;
   3;0,1,2;,
   3;3,2,1;,
   3;1,4,3;,
   3;5,3,4;,
   3;4,6,5;,
   3;7,5,6;;

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
    40;
    1058861103,
    993345886,
    1061830791,
    973283200,
    973280256,
    1059308801,
    1006239745,
    1061470401,
    1056957793,
    973280256,
    1058863099,
    993344499,
    1061829242,
    973283200,
    1065344827,
    1059216015,
    1004191765,
    1061547219,
    1056957793,
    1065344827,
    1057853045,
    1051646897,
    1061326739,
    1061149604,
    973280256,
    1058490378,
    1048528346,
    1061447973,
    1061149604,
    1065344827,
    1048628107,
    1061539443,
    1058354176,
    1065338007,
    973280256,
    1048597505,
    1061543191,
    1058355797,
    1065338007,
    1065344827;
   }
  }
 }
}