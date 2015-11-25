#include <mapnik/datasource.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/params.hpp>
#include <mapnik/query.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/box2d.hpp>
#include <mapnik/coord.hpp>
#include <mapnik/feature_layer_desc.hpp>

#include "gtest/gtest.h"

using mapnik::parameters;
using mapnik::datasource;
using mapnik::datasource_cache;
using mapnik::box2d;
using mapnik::coord2d;
using mapnik::query;
using mapnik::featureset_ptr;
using mapnik::feature_ptr;
using mapnik::layer_descriptor;
using mapnik::datasource_geometry_t;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

TEST(geowave_plugin, envelope)
{
   parameters p;
   p["type"]="geowave";
   p["zookeeper_url"]="localhost:2181";
   p["instance_name"]="geowave";
   p["username"]="root";
   p["password"]="password";
   p["table_namespace"]="mapnik";
   p["adapter_id"]="world_merc";

   std::shared_ptr<datasource> ds = datasource_cache::instance().create(p);

   box2d<double> env = ds->envelope();

   EXPECT_NEAR(-180.0, env.minx(), 0.0001);
   EXPECT_NEAR(-59.641, env.miny(), 0.0001);
   EXPECT_NEAR(180.0, env.maxx(), 0.0001);
   EXPECT_NEAR(83.613, env.maxy(), 0.0001);
}

TEST(geowave_plugin, combined_envelope)
{
   parameters p;
   p["type"]="geowave";
   p["zookeeper_url"]="localhost:2181";
   p["instance_name"]="geowave";
   p["username"]="root";
   p["password"]="password";
   p["table_namespace"]="mapnik";
   p["adapter_id"]="world_merc";
   p["cql_filter"]="BBOX(the_geom, -180.0, 0.0, 180.0, 83.613)";

   std::shared_ptr<datasource> ds = datasource_cache::instance().create(p);

   box2d<double> env = ds->envelope();

   EXPECT_NEAR(-180.0, env.minx(), 0.01);
   EXPECT_NEAR(0.0, env.miny(), 0.01);
   EXPECT_NEAR(180.0, env.maxx(), 0.01);
   EXPECT_NEAR(83.613, env.maxy(), 0.0001);
}

TEST(geowave_plugin, features)
{
   parameters p;
   p["type"]="geowave";
   p["zookeeper_url"]="localhost:2181";
   p["instance_name"]="geowave";
   p["username"]="root";
   p["password"]="password";
   p["table_namespace"]="mapnik";
   p["adapter_id"]="world_merc";

   std::shared_ptr<datasource> ds = datasource_cache::instance().create(p);

   featureset_ptr fs = ds->features(query(box2d<double>(-180.0, -90.0, 180.0, 90.0)));

   EXPECT_NE(feature_ptr(), fs->next());
}

TEST(geowave_plugin, features_at_point)
{
   parameters p;
   p["type"]="geowave";
   p["zookeeper_url"]="localhost:2181";
   p["instance_name"]="geowave";
   p["username"]="root";
   p["password"]="password";
   p["table_namespace"]="mapnik";
   p["adapter_id"]="world_merc";

   std::shared_ptr<datasource> ds = datasource_cache::instance().create(p);

   featureset_ptr fs = ds->features_at_point(coord2d(-77.0164, 38.9047), 0.5);

   feature_ptr feature = fs->next();

   EXPECT_EQ(feature->get("FIPS").to_string(), "US");
   EXPECT_EQ(feature->get("ISO2").to_string(), "US");
   EXPECT_EQ(feature->get("ISO3").to_string(), "USA");
   EXPECT_EQ(feature->get("UN").to_int(), 840);
   EXPECT_EQ(feature->get("NAME").to_string(), "United States");
   EXPECT_EQ(feature->get("AREA").to_int(), 915896);
   EXPECT_EQ(feature->get("POP2005").to_int(), 299846449);
   EXPECT_EQ(feature->get("REGION").to_int(), 19);
   EXPECT_EQ(feature->get("SUBREGION").to_int(), 21);
   EXPECT_EQ(feature->get("LON").to_double(), -98.606000);
   EXPECT_EQ(feature->get("LAT").to_double(), 39.622000);
}

TEST(geowave_plugin, cql_query_filter)
{
   parameters p;
   p["type"]="geowave";
   p["zookeeper_url"]="localhost:2181";
   p["instance_name"]="geowave";
   p["username"]="root";
   p["password"]="password";
   p["table_namespace"]="mapnik";
   p["adapter_id"]="world_merc";
   p["cql_filter"]="BBOX(the_geom, -6.7597, 52.8478, -5.7597, 53.8478)";

   std::shared_ptr<datasource> ds = datasource_cache::instance().create(p);

   box2d<double> env = ds->envelope();

   EXPECT_NEAR(-6.7597, env.minx(), 0.00001);
   EXPECT_NEAR(52.8478, env.miny(), 0.00001);
   EXPECT_NEAR(-5.7597, env.maxx(), 0.00001);
   EXPECT_NEAR(53.8478, env.maxy(), 0.00001);

   featureset_ptr fs = ds->features(query(box2d<double>(-180.0, -90.0, 180.0, 90.0)));

   feature_ptr feature = fs->next();

   EXPECT_EQ(feature->get("FIPS").to_string(), "EI");
   EXPECT_EQ(feature->get("ISO2").to_string(), "IE");
   EXPECT_EQ(feature->get("ISO3").to_string(), "IRL");
   EXPECT_EQ(feature->get("UN").to_int(), 372);
   EXPECT_EQ(feature->get("NAME").to_string(), "Ireland");
   EXPECT_EQ(feature->get("AREA").to_int(), 6889);
   EXPECT_EQ(feature->get("POP2005").to_int(), 4143294);
   EXPECT_EQ(feature->get("REGION").to_int(), 150);
   EXPECT_EQ(feature->get("SUBREGION").to_int(), 154);
   EXPECT_EQ(feature->get("LON").to_double(), -8.152000);
   EXPECT_EQ(feature->get("LAT").to_double(), 53.177000);
}

TEST(geowave_plugin, fields_and_types)
{
   parameters p;
   p["type"]="geowave";
   p["zookeeper_url"]="localhost:2181";
   p["instance_name"]="geowave";
   p["username"]="root";
   p["password"]="password";
   p["table_namespace"]="mapnik";
   p["adapter_id"]="world_merc";

   std::shared_ptr<datasource> ds = datasource_cache::instance().create(p);

   layer_descriptor ld = ds->get_descriptor();

   EXPECT_EQ("geowave", ld.get_name());

   EXPECT_EQ(11, ld.get_descriptors().size());

   for (auto attrib : ld.get_descriptors())
   {
      if (attrib.get_name() == "FIPS")
         EXPECT_EQ(mapnik::String, attrib.get_type());
      else if (attrib.get_name() == "ISO2")
         EXPECT_EQ(mapnik::String, attrib.get_type());
      else if (attrib.get_name() == "ISO3")
         EXPECT_EQ(mapnik::String, attrib.get_type());
      else if (attrib.get_name() == "UN")
         EXPECT_EQ(mapnik::Integer, attrib.get_type());
      else if (attrib.get_name() == "NAME")
         EXPECT_EQ(mapnik::String, attrib.get_type());
      else if (attrib.get_name() == "AREA")
         EXPECT_EQ(mapnik::Integer, attrib.get_type());
      else if (attrib.get_name() == "POP2005")
         EXPECT_EQ(mapnik::Integer, attrib.get_type());
      else if (attrib.get_name() == "REGION")
         EXPECT_EQ(mapnik::Integer, attrib.get_type());
      else if (attrib.get_name() == "SUBREGION")
         EXPECT_EQ(mapnik::Integer, attrib.get_type());
      else if (attrib.get_name() == "LON")
         EXPECT_EQ(mapnik::Double, attrib.get_type());
      else if (attrib.get_name() == "LAT")
         EXPECT_EQ(mapnik::Double, attrib.get_type());
      else
         EXPECT_EQ(NULL, attrib.get_type());
   }
}

TEST(geowave_plugin, geometry_detection)
{
   parameters p;
   p["type"]="geowave";
   p["zookeeper_url"]="localhost:2181";
   p["instance_name"]="geowave";
   p["username"]="root";
   p["password"]="password";
   p["table_namespace"]="mapnik";
   p["adapter_id"]="world_merc";

   std::shared_ptr<datasource> ds = datasource_cache::instance().create(p);

   EXPECT_EQ(datasource_geometry_t::Collection, ds->get_geometry_type());
}

int main(int argc, char **argv) {
  if (datasource_cache::instance().register_datasource(std::string(argv[1]))){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }
  return -1;
}
