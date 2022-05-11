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

 Frame FX_Lightning_Line_4_0 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh FX_Lightning_Line_4_0 {
   14;
   -0.261628;-0.135024;-0.093199;,
   -0.148971;-0.118187;-0.237883;,
   -0.339473;-0.152982;-0.049964;,
   -0.150985;-0.201782;-0.228354;,
   -0.223551;-0.009342;-0.054450;,
   -0.261628;-0.135024;-0.093199;,
   -0.236195;-0.009077;0.049575;,
   -0.339473;-0.152982;-0.049964;,
   -0.236195;-0.009077;0.049575;,
   0.078437;0.201782;0.194646;,
   -0.223551;-0.009342;-0.054450;,
   0.095592;0.184940;0.057561;,
   0.334628;0.189879;0.237883;,
   0.339473;0.182240;0.185064;;
   8;
   3;0,1,2;,
   3;3,2,1;,
   3;4,5,6;,
   3;7,6,5;,
   3;8,9,10;,
   3;11,10,9;,
   3;9,12,11;,
   3;13,11,12;;

   MeshMaterialList {
    1;
    8;
    0,
    0,
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
    70;
    3205326996,
    1009815734,
    3210043873,
    1048552096,
    1065344827,
    3204920554,
    3176943789,
    3210277418,
    973279841,
    1065344827,
    3205480189,
    1025082090,
    3209928066,
    1048552096,
    973280256,
    3204916815,
    3176942912,
    3210279751,
    973279841,
    973280256,
    3207110646,
    1050660083,
    3207545775,
    1056923930,
    1065344827,
    3205326996,
    1009815734,
    3210043873,
    1048552096,
    1065344827,
    3207003606,
    1049879355,
    3207816401,
    1056923930,
    973280256,
    3205480189,
    1025082090,
    3209928066,
    1048552096,
    973280256,
    3207003606,
    1049879355,
    3207816401,
    1056923930,
    973280256,
    3198134993,
    1062013885,
    3204634299,
    1061124210,
    973280256,
    3207110646,
    1050660083,
    3207545775,
    1056923930,
    1065344827,
    3200912037,
    1061397715,
    3204618129,
    1061124210,
    1065344827,
    1043291357,
    1063332054,
    3202567871,
    1065304149,
    973280256,
    1043268414,
    1063329067,
    3202584119,
    1065304149,
    1065344827;
   }
  }
 }
}