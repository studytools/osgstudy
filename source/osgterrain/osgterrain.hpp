#include <osgGA/GUIEventHandler>

#include <osgTerrain/Terrain>
#include <osgTerrain/TerrainTile>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/DisplacementMappingTechnique>
#include <osgTerrain/Layer>
#include <osgTerrain/Locator>

#include <gdal_priv.h>

class DEMTerrain
{
public:
  typedef unsigned int NodeMask;

  DEMTerrain(NodeMask node_mask);
  ~DEMTerrain();

  int ReadDEMFile(const std::string& file);

  void setlocator(osg::ref_ptr<osgTerrain::Locator> locator);

  osg::ref_ptr<osgTerrain::TerrainTile> terrain_tile() const;

private:
  void SetLocator(const int &w,const int &h,const double(&geo_transform)[6]);
  void SetHeightField(const int &w,const int &h,const double(&geo_transform)[6]);

private:
  osg::ref_ptr<osgTerrain::HeightFieldLayer> height_field_layer_;
  osg::ref_ptr<osgTerrain::TerrainTile> terrain_tile_;
  GDALDataset* dataset_;
};

