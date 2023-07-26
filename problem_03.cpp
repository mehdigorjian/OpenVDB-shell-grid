#include <fstream>
#include <iostream>
#include <math.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/LevelSetSphere.h>
/*
  Write function shell_grid that creates the 'shell indicator' grid of a level set

  This function takes in a narrow band level set grid pointer as input

  It should return an integer grid pointer that stores the following values:
    * voxel position xyz stores value 1 if xyz lies in the active band of the level set
    * voxel position xyz stores value 2 if xyz stores a negative value in the level set
    * voxel position xyz stores value 0 otherwise

  In short the integer grid represents an object with the following values:
    * 'fuzzy boundary' has value 1
    * 'inside' has value 2
    * 'outside' has value 0

  The topology of the integer grid's tree should be the same as the scalar grid's tree

  This function is templated on
    * ScalarGridPtr, the scalar grid pointer input type (e.g. float grid, double grid, etc.)
    * IntGridPtr, the integer grid pointer input type (e.g. Int32 grid, Int64 grid, etc.)

  This function should assert that both ScalarGridPtr and IntGridPtr represent valid scalar and int types, respectively

*/
// template <typename ScalarGridPtr, typename IntGridPtr>
// IntGridPtr shell_grid(ScalarGridPtr grid)
openvdb::Int32Grid::Ptr shell_grid(openvdb::FloatGrid::Ptr grid)
{
    std::cout << "We are processing the grid shell, please be patient!" << std::endl;
    openvdb::Int32Grid::Ptr girdShell = openvdb::Int32Grid::create(0);
    int boundry_count = 0, inside_count = 0, outside_count = 0, total = 0;

    for (auto iter = grid->cbeginValueOn(); iter; ++iter)
    {
        total++;
        openvdb::Vec3d worldSpacePos = iter.getCoord().asVec3d();
        openvdb::Coord xyz = girdShell->transform().worldToIndexNodeCentered(worldSpacePos);

        if (iter.getValue() < 0)
        {
            girdShell->tree().setValue(xyz, 2);
            inside_count++;
        }
        else if (iter.getValue() == 0)
        {
            girdShell->tree().setValue(xyz, 1);
            boundry_count++;
        }
        else
        {
            girdShell->tree().setValue(xyz, 0);
            outside_count++;
        }
    }
    std::ofstream myFile("../output_data/output_info.txt");
    std::cout << "Boundry num (value==1)   " << boundry_count << "\n"
              << "Inside num  (value==2)   " << inside_count << "\n"
              << "Outside num (value==0)   " << outside_count << "\n"
              << "Total                    " << total << std::endl;

    myFile << "Boundry num (value==1)   " << boundry_count << "\n"
           << "Inside num  (value==2)   " << inside_count << "\n"
           << "Outside num (value==0)   " << outside_count << "\n"
           << "Total                    " << total << std::endl;
    myFile.close();
    std::cout << "The output .txt file was added to the 'output_data' directory!" << std::endl;
    return girdShell;
}

int main()
{
    openvdb::initialize();
    using IntGridPtr = openvdb::Int32Grid::Ptr;
    using ScalarGridPtr = openvdb::FloatGrid::Ptr;
    // using ScalarGridPtr = openvdb::ScalarGrid::Ptr;

    // Create a float grid level set sphere in OpenVDB with
    // center (0, 0, 0)
    // radius 50 (in index space)
    // voxel size 0.1
    // half width 3.0
    // Create a FloatGrid and populate it with a narrow-band
    // signed distance field of a sphere.
    openvdb::FloatGrid::Ptr grid = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(
        /*radius=*/50.0, /*center=*/openvdb::Vec3f(0.0, 0.0, 0.0),
        /*voxel size=*/0.1, /*width=*/3.0);

    grid->setName("grid_shell");
    // openvdb::io::File("../Level_set_sphere.vdb").write({grid});

    // create an integer grid by calling shell_grid on the level set sphere
    IntGridPtr shell = shell_grid(grid);

    openvdb::io::File file("../output_data/grid_shell.vdb");
    openvdb::GridPtrVec grids;
    grids.push_back(shell);
    file.write(grids);
    file.close();

    // print the number of voxels with value 1
    // print the number of voxels with value 2
    std::cout << "Grid shell processed and the .vdb file created!" << std::endl;

    return 0;
}
