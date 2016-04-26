#include "osgterrain.hpp"

DEMTerrain::DEMTerrain(NodeMask node_mask)
  : height_field_layer_(new osgTerrain::HeightFieldLayer)
  , terrain_tile_(new osgTerrain::TerrainTile)
{
  osg::ref_ptr<osgTerrain::ValidDataOperator> validDataOperator =
  new osgTerrain::NoDataValue(-32767.0000);
  height_field_layer_->setValidDataOperator(validDataOperator.get());

  terrain_tile_->setDataVariance(osg::Object::DYNAMIC);
  terrain_tile_->setTreatBoundariesToValidDataAsDefaultValue(false);
  terrain_tile_->setRequiresNormals(false);
  terrain_tile_->setNodeMask(node_mask);
}

DEMTerrain::~DEMTerrain()
{
}

osg::ref_ptr<osgTerrain::TerrainTile> DEMTerrain::terrain_tile() const
{
  return terrain_tile_;
}

int DEMTerrain::ReadDEMFile(const std::string& file)
{ 
  GDALAllRegister();
  dataset_ = (GDALDataset*)(GDALOpen(file.c_str(),GA_ReadOnly));
  if (dataset_ == NULL) return -1;

  double geoTransform[6];
  int w = dataset_->GetRasterXSize();
  int h = dataset_->GetRasterYSize();
  if(dataset_->GetGeoTransform(geoTransform)==CE_None)
  {
    SetLocator(w,h,geoTransform);
    SetHeightField(w,h,geoTransform);
  }
  return 0;
}

void DEMTerrain::setlocator(osg::ref_ptr<osgTerrain::Locator> locator)
{
  height_field_layer_->setLocator(locator.get());
}

void DEMTerrain::SetLocator(
  const int &w, const int &h, const double (&geo_transform)[6])
{
  const char* pszSourceSRS = dataset_->GetProjectionRef();
  if(!pszSourceSRS || strlen(pszSourceSRS)==0)
    pszSourceSRS = dataset_->GetGCPProjection();

  osg::ref_ptr<osgTerrain::Locator> locator = new osgTerrain::Locator;

  if(pszSourceSRS)
  {
    locator->setFormat("WKT");
    locator->setCoordinateSystem(pszSourceSRS);
  }

  osg::Matrixd matrix;
  matrix.set(geo_transform[1], geo_transform[2], 0.0,0.0,
             geo_transform[4], std::abs(geo_transform[5]), 0.0,0.0,
             0.0,              0.0,              1.0,0.0,
             geo_transform[0], geo_transform[3], 0.0,1.0);

  locator->setTransform(
      osg::Matrixd::scale(static_cast<double>(w-1),
      static_cast<double>(h-1),1.0) * 
      osg::Matrixd::translate(0,static_cast<double>(h-1)*geo_transform[5]*2,0) * 
      matrix);

  locator->setDefinedInFile(true);

  height_field_layer_->setLocator(locator.get());
}   

void DEMTerrain::SetHeightField(
  const int &w, const int &h, const double (&geo_transform)[6])
{
  int windowX = 0;
  int windowY = 0;
  int windowWidth = w;
  int windowHeight = h;

  int destX = 0;
  int destY = 0;
  int destWidth = osg::minimum(w,4096);
  int destHeight = osg::minimum(h,4096);

  double TopLeft[2],BottomLeft[2],BottomRight[2],TopRight[2];
  TopLeft[0] = geo_transform[0];
  TopLeft[1] = geo_transform[3];
  BottomLeft[0] = TopLeft[0]+geo_transform[2]*(h-1);
  BottomLeft[1] = TopLeft[1]+geo_transform[5]*(h-1);
  BottomRight[0] = BottomLeft[0]+geo_transform[1]*(w-1);
  BottomRight[1] = BottomLeft[1]+geo_transform[4]*(w-1);
  TopRight[0] = TopLeft[0]+geo_transform[1]*(w-1);
  TopRight[1] = TopLeft[1]+geo_transform[4]*(w-1);

  double rotation = std::atan2(geo_transform[2],geo_transform[1]);

  int numBands = dataset_->GetRasterCount();
  GDALRasterBand* bandGray = 0;
  GDALRasterBand* bandRed = 0;
  GDALRasterBand* bandGreen = 0;
  GDALRasterBand* bandBlue = 0;
  GDALRasterBand* bandAlpha = 0;
  for(int b=1; b<=numBands; ++b)
  {
    GDALRasterBand* band = dataset_->GetRasterBand(b);
    if(band->GetColorInterpretation()==GCI_GrayIndex) bandGray = band;
    else if(band->GetColorInterpretation()==GCI_RedBand) bandRed = band;
    else if(band->GetColorInterpretation()==GCI_GreenBand) bandGreen = band;
    else if(band->GetColorInterpretation()==GCI_BlueBand) bandBlue = band;
    else if(band->GetColorInterpretation()==GCI_AlphaBand) bandAlpha = band;
    else bandGray = band;
  }
  GDALRasterBand* bandSelected = 0;
  if(!bandSelected && bandGray) bandSelected = bandGray;
  else if(!bandSelected && bandAlpha) bandSelected = bandAlpha;
  else if(!bandSelected && bandRed) bandSelected = bandRed;
  else if(!bandSelected && bandGreen) bandSelected = bandGreen;
  else if(!bandSelected && bandBlue) bandSelected = bandBlue;

  if(bandSelected)
  {
    osg::HeightField* hf = new osg::HeightField;
    hf->allocate(destWidth,destHeight);

    bandSelected->RasterIO(GF_Read,windowX,windowY,windowWidth,windowHeight,
      (void*)(&(hf->getHeightList().front())),destWidth,destHeight,GDT_Float32,0,0);
    unsigned int copy_r = hf->getNumRows()-1;

    for(unsigned int r=0; r<copy_r; ++r,--copy_r)
    {
      for(unsigned int c=0; c<hf->getNumColumns(); ++c)
      {
        float temp = hf->getHeight(c,r);
        hf->setHeight(c,r,hf->getHeight(c,copy_r));
        hf->setHeight(c,copy_r,temp);
      }
    }


    hf->setXInterval(sqrt(geo_transform[1]*geo_transform[1] + geo_transform[2]*geo_transform[2]));
    hf->setYInterval(sqrt(geo_transform[4]*geo_transform[4] + geo_transform[5]*geo_transform[5]));
    hf->setRotation(osg::Quat(rotation,osg::Vec3d(0.0,0.0,1.0)));

    hf->setOrigin(osg::Vec3(BottomLeft[0],BottomLeft[1],0));

    height_field_layer_->setHeightField(hf);

  }


  terrain_tile_->setElevationLayer(height_field_layer_);
}

