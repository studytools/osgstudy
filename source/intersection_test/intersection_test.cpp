#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgUtil/IntersectionVisitor>
#include <ctime> 
#include <cstdlib> 

#define NUMBER_O_SHRUBS 1250

osg::Drawable* createShrub(const float & scale,osg::StateSet* bbState)
{
  float width = 1.5f;
  float height = 3.0f;

  width *= scale;
  height *= scale;

  osg::Geometry* shrubQuad = new osg::Geometry;

  osg::Vec3Array* shrubVerts = new osg::Vec3Array(4);
  (*shrubVerts)[0] = osg::Vec3(-width/2.0f,0,0);
  (*shrubVerts)[1] = osg::Vec3(width/2.0f,0,0);
  (*shrubVerts)[2] = osg::Vec3(width/2.0f,0,height);
  (*shrubVerts)[3] = osg::Vec3(-width/2.0f,0,height);

  shrubQuad->setVertexArray(shrubVerts);

  osg::Vec2Array* shrubTexCoords = new osg::Vec2Array(4);
  (*shrubTexCoords)[0].set(0.0f,0.0f);
  (*shrubTexCoords)[1].set(1.0f,0.0f);
  (*shrubTexCoords)[2].set(1.0f,1.0f);
  (*shrubTexCoords)[3].set(0.0f,1.0f);
  shrubQuad->setTexCoordArray(0,shrubTexCoords);

  shrubQuad->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

  shrubQuad->setStateSet(bbState);

  return shrubQuad;
}

int main(int argc,char **argv)
{
  osgViewer::Viewer viewer;

  osg::Group* rootNode = new osg::Group();
  osg::Node* tankNode = NULL;
  osg::Node* terrainNode = NULL;
  osg::PositionAttitudeTransform* tankXform =
     new osg::PositionAttitudeTransform();

  tankNode = osgDB::readNodeFile("\\Models\\T72-Tank\\T72-tank_des.flt");
  terrainNode = osgDB::readNodeFile("\\Models\\JoeDirt\\JoeDirt.flt");
  if(!(tankNode && terrainNode))
  {
    std::cout << "Couldn't load models, quitting." << std::endl;
    return -1;
  }
  rootNode->addChild(terrainNode);
  rootNode->addChild(tankXform);
  tankXform->addChild(tankNode);
  double tankXPosition = -10.0;
  double tankYPosition = -10.0;

  osgUtil::LineSegmentIntersector* tankLocationSegment = new osg::LineSegmentIntersector();
  tankLocationSegment->set(
     osg::Vec3(tankXPosition,tankYPosition,999),
     osg::Vec3(tankXPosition,tankYPosition,-999));

  osgUtil::IntersectVisitor findTankElevationVisitor;
  findTankElevationVisitor.addLineSegment(tankLocationSegment);
  terrainNode->accept(findTankElevationVisitor);

  osgUtil::IntersectVisitor::HitList tankElevationLocatorHits;
  tankElevationLocatorHits =
     findTankElevationVisitor.getHitList(tankLocationSegment);
  osgUtil::Hit heightTestResults;
  if(tankElevationLocatorHits.empty())
  {
    std::cout << " couldn't place tank on terrain" << std::endl;
    return -1;
  }
  heightTestResults = tankElevationLocatorHits.front();
  osg::Vec3d terrainHeight = heightTestResults.getWorldIntersectPoint();

  tankXform->setPosition(terrainHeight);
  tankXform->setAttitude(
     osg::Quat(osg::DegreesToRadians(-45.0),osg::Vec3(0,0,1)));
  viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);

  viewer.setSceneData(rootNode);
  viewer.realize();

  osg::Billboard* shrubBillBoard = new osg::Billboard();
  rootNode->addChild(shrubBillBoard);

  shrubBillBoard->setMode(osg::Billboard::AXIAL_ROT);
  shrubBillBoard->setAxis(osg::Vec3(0.0f,0.0f,1.0f));
  shrubBillBoard->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));

  osg::Texture2D *ocotilloTexture = new osg::Texture2D;
  ocotilloTexture->setImage(osgDB::readImageFile("images\\ocotillo.png"));

  osg::StateSet* billBoardStateSet = new osg::StateSet;

  billBoardStateSet->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
  billBoardStateSet->setTextureAttributeAndModes(0,ocotilloTexture,osg::StateAttribute::ON);
  billBoardStateSet->setAttributeAndModes(new osg::BlendFunc,osg::StateAttribute::ON);

  osg::AlphaFunc* alphaFunction = new osg::AlphaFunc;
  alphaFunction->setFunction(osg::AlphaFunc::GEQUAL,0.05f);
  billBoardStateSet->setAttributeAndModes(alphaFunction,osg::StateAttribute::ON);
  srand(time(0)); // Initialize random number generator.

  osgUtil::IntersectVisitor isectVisitor;
  osg::LineSegment* terrainIsect[NUMBER_O_SHRUBS];

  int randomX,randomY;

  for(int i=0; i< NUMBER_O_SHRUBS; i++)
  {
    randomX = (rand() % 100) + 1;
    randomY = (rand() % 100) + 1;
    terrainIsect[i] = new osg::LineSegment(
       osg::Vec3(randomX,randomY,999),
       osg::Vec3(randomX,randomY,-999));
    isectVisitor.addLineSegment(terrainIsect[i]);
  }
  terrainNode->accept(isectVisitor);

  osg::Drawable* shrubDrawable[NUMBER_O_SHRUBS];

  for(int j = 0; j < NUMBER_O_SHRUBS; j++)
  {
    float randomScale = ((rand() % 15) + 1) / 10.0;
    shrubDrawable[j] = createShrub(randomScale,billBoardStateSet);
    osgUtil::IntersectVisitor::HitList hitList = isectVisitor.getHitList(terrainIsect[j]);
    if(!hitList.empty())
    {
      osgUtil::Hit firstHit = hitList.front();
      osg::Vec3d shrubPosition = firstHit.getWorldIntersectPoint();

      // osg::Vec3d shrubPosition =
      // isectVisitor.getHitList(terrainIsect[j]).front().getWorldIntersectPoint();

      shrubBillBoard->addDrawable(shrubDrawable[j],shrubPosition);
    }
  }

  while(!viewer.done())
  {
    // wait for all cull and draw threads to complete.
    viewer.sync();

    // update the scene by traversing it with the update visitor which will
    // call all node update callbacks and animations.
    viewer.update();

    // fire off the cull and draw traversals of the scene.
    viewer.frame();
  }
  // wait for all cull and draw threads to complete before exit.
  viewer.sync();
  return 0;
}