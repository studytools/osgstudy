#include <osg/ArgumentParser>
#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>

#include <osgViewer/Viewer>

#include <osgGA/TrackballManipulator>

#include <osgTerrain/Terrain>
#include <osgTerrain/TerrainTile>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/DisplacementMappingTechnique>
#include <osgTerrain/Layer>
#include <osgTerrain/Locator>

int main(int argc,char** argv)
{
  osg::ArgumentParser arguments(&argc,argv);

  // create the window to draw to.
  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
  traits->x = 200;
  traits->y = 200;
  traits->width = 800;
  traits->height = 600;
  traits->windowDecoration = true;
  traits->doubleBuffer = true;
  traits->sharedContext = 0;
  osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
  osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
  if(!gw)
  {
    osg::notify(osg::NOTICE)<<"Error: unable to create graphics window."<<std::endl;
    return 1;
  }
  osg::ref_ptr<osgTerrain::HeightFieldLayer> hfl = new osgTerrain::HeightFieldLayer;
  osg::ref_ptr<osgTerrain::TerrainTile> tt = new osgTerrain::TerrainTile;


  osgDB::ReaderWriter* rw = osgDB::Registry::instance()->getReaderWriterForExtension("gdal");
  osg::ref_ptr<osg::HeightField> hf = rw->readHeightField("dem.tif").getHeightField();

//   osg::ref_ptr<osgTerrain::Layer> layer = osgDB::readFile<osgTerrain::Layer>("dem.tif");
//   osg::ref_ptr<osg::HeightField> hf = osgDB::readHeightFieldFile("dem.tif");

  osg::HeightField::HeightList hl = hf->getHeightList();
  float maxz = *std::max_element(hl.begin(), hl.end());
  osg::Vec3 ori = hf->getOrigin();
  float xd = hf->getXInterval();
  float yd = hf->getYInterval();
  int nr = hf->getNumRows();
  int nc = hf->getNumColumns();
  float minx = ori[0];
  float maxx = minx + xd*nr;
  float miny = ori[1];
  float maxy = miny + yd*nc;
  hfl->setHeightField(hf.get());

  osg::ref_ptr<osgTerrain::Locator> locator = new osgTerrain::Locator;
//   locator->setCoordinateSystemType(osgTerrain::Locator::PROJECTED);
//   locator->setFormat("WKT");
  locator->setTransformAsExtents(minx,miny,maxx,maxy);

  hfl->setLocator(locator.get());

  osg::ref_ptr<osgTerrain::ValidDataOperator> validDataOperator = new osgTerrain::ValidRange(0.0,maxz);
  hfl->setValidDataOperator(validDataOperator.get());

  tt->setDataVariance(osg::Object::DYNAMIC);
  tt->setTreatBoundariesToValidDataAsDefaultValue(false);
  tt->setElevationLayer(hfl);
  tt->setRequiresNormals(false);

//   osg::ref_ptr<osgTerrain::GeometryTechnique> geometryTechnique = 
//     new osgTerrain::GeometryTechnique;
//   tt->setTerrainTechnique(geometryTechnique.get());

//   osg::BoundingSphere bs = tt->computeBound();

  osgViewer::Viewer viewer(arguments);
  viewer.getCamera()->setGraphicsContext(gc.get());
  viewer.getCamera()->setViewport(0,0,800,600);

//   osg::ref_ptr<osg::Node> rootnode = osgDB::readRefNodeFiles(arguments);
  viewer.setSceneData( tt.get() );

  viewer.setCameraManipulator( new osgGA::TrackballManipulator );
  viewer.realize();

  while(!viewer.done())
  {
    viewer.frame();
  }

  return 0;
}
