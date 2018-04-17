/*////////////////////////////////////////////////////////////////////////////
 Copyright 2018 Istituto Nazionale di Fisica Nucleare

 Licensed under the EUPL, Version 1.2 or - as soon they will be approved by
 the European Commission - subsequent versions of the EUPL (the "Licence").
 You may not use this work except in compliance with the Licence.

 You may obtain a copy of the Licence at:

 https://joinup.ec.europa.eu/software/page/eupl

 Unless required by applicable law or agreed to in writing, software
 distributed under the Licence is distributed on an "AS IS" basis, WITHOUT
 WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 Licence for the specific language governing permissions and limitations under
 the Licence.
////////////////////////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <stdarg.h>
#include <string>

#include <IBVoxCollection.h>
#include <IBVoxFilters.h>
#include <IBSubImageGrabber.h>


#define BEGIN_TESTING(name)                \
static int _fail = 0;                      \
printf("..:: Testing " #name " ::..\n");

#define TEST1(val) _fail += (val)==0
#define TEST0(val) _fail += (val)!=0
#define END_TESTING return _fail;



void SaveContainerSet(IBVoxCollection &voxels,
                      const char *filename,
                      int index)
{


    char name[300];
    sprintf(name,"%s_%i.vtk",filename,index);

    // image //
    voxels.ExportToVtk(name);

    // blocco piccolo fila centrale //
    Vector3i p0,p1;
    IBSubImageGrabber<IBVoxCollection> grabber(voxels);
    p0 << 30,25,20;
    p1 << 50,45,40;
    IBVoxCollection img = grabber.GrabRegion<IBVoxCollection>(p0,p1);
    sprintf(name,"%s_ss_%i.vtk",filename,index);
    img.ExportToVtk(name);

    // filter //
    IBVoxFilter_Abtrim trim(Vector3i(5,5,5));
    IBFilterGaussShape shape(0.2);
    trim.SetKernelSpherical(shape);
    trim.SetABTrim(0,2);

    IBVoxCollection filtered = voxels;
    trim.SetImage(&filtered);
    trim.Run();
    sprintf(name,"%s_trimm501_%i.vtk",filename,index);
    filtered.ExportToVtk(name);

    filtered = img;
    trim.SetImage(&filtered);
    trim.Run();
    sprintf(name,"%s_ss_trimm501_%i.vtk",filename,index);
    filtered.ExportToVtk(name);

}

std::string GetFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

std::string FileNameRemoveExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(0,FileName.find_last_of("."));
    return "";
}
