// plugin
#include "geowave_datasource.hpp"
#include "geowave_featureset.hpp"

// mapnik
#include <mapnik/debug.hpp>
#include <mapnik/csv/csv_grammar.hpp>

// jace
#include <jace/Jace.h>
using jace::java_cast;
using jace::java_new;
using jace::instanceof;

#include <jace/JClass.h>
using jace::JClass;
#include <jace/JClassImpl.h>
using jace::JClassImpl;

#include "jace/JNIException.h"
using jace::JNIException;

#include "jace/VirtualMachineShutdownError.h"
using jace::VirtualMachineShutdownError;

#include "jace/OptionList.h"
using jace::OptionList;
using jace::Option;
using jace::ClassPath;
using jace::Verbose;
using jace::CustomOption;

#include <jace/StaticVmLoader.h>
using jace::StaticVmLoader;

#ifdef _WIN32
#include "jace/Win32VmLoader.h"
using jace::Win32VmLoader;
const std::string os_pathsep(";");
#else
#include "jace/UnixVmLoader.h"
using ::jace::UnixVmLoader;
const std::string os_pathsep(":");
#endif

// GeoWave
#include "jace/JArray.h"
using jace::JArray;

#include "jace/proxy/types/JChar.h"
using jace::proxy::types::JChar;
#include "jace/proxy/types/JDouble.h"
using jace::proxy::types::JDouble;

#include "jace/proxy/java/lang/Boolean.h"
using jace::proxy::java::lang::Boolean;
#include "jace/proxy/java/lang/Class.h"
using jace::proxy::java::lang::Class;
#include "jace/proxy/java/lang/Double.h"
using jace::proxy::java::lang::Double;
#include "jace/proxy/java/lang/Float.h"
using jace::proxy::java::lang::Float;
#include "jace/proxy/java/lang/Integer.h"
using jace::proxy::java::lang::Integer;
#include "jace/proxy/java/lang/Long.h"
using jace::proxy::java::lang::Long;
#include "jace/proxy/java/lang/Short.h"
using jace::proxy::java::lang::Short;
#include "jace/proxy/java/lang/String.h"
using jace::proxy::java::lang::String;
#include "jace/proxy/java/util/Date.h"
using jace::proxy::java::util::Date;
#include "jace/proxy/java/util/List.h"
using jace::proxy::java::util::List;

#include "jace/proxy/com/vividsolutions/jts/geom/Coordinate.h"
using jace::proxy::com::vividsolutions::jts::geom::Coordinate;
#include "jace/proxy/com/vividsolutions/jts/geom/Envelope.h"
using jace::proxy::com::vividsolutions::jts::geom::Envelope;
#include "jace/proxy/com/vividsolutions/jts/geom/Geometry.h"
using jace::proxy::com::vividsolutions::jts::geom::Geometry;
#include "jace/proxy/com/vividsolutions/jts/geom/GeometryCollection.h"
using jace::proxy::com::vividsolutions::jts::geom::GeometryCollection;
#include "jace/proxy/com/vividsolutions/jts/geom/GeometryFactory.h"
using jace::proxy::com::vividsolutions::jts::geom::GeometryFactory;

#include "jace/proxy/org/apache/accumulo/core/client/AccumuloException.h"
using jace::proxy::org::apache::accumulo::core::client::AccumuloException;
#include "jace/proxy/org/apache/accumulo/core/client/AccumuloSecurityException.h"
using jace::proxy::org::apache::accumulo::core::client::AccumuloSecurityException;

#include "jace/proxy/org/opengis/feature/simple/SimpleFeatureType.h"
using jace::proxy::org::opengis::feature::simple::SimpleFeatureType;
#include "jace/proxy/org/opengis/feature/type/GeometryDescriptor.h"
using jace::proxy::org::opengis::feature::type::GeometryDescriptor;
#include "jace/proxy/org/opengis/feature/type/GeometryType.h"
using jace::proxy::org::opengis::feature::type::GeometryType;
#include "jace/proxy/org/opengis/feature/type/AttributeDescriptor.h"
using jace::proxy::org::opengis::feature::type::AttributeDescriptor;
#include "jace/proxy/org/opengis/feature/type/AttributeType.h"
using jace::proxy::org::opengis::feature::type::AttributeType;

#include "jace/proxy/org/geotools/filter/text/cql2/CQLException.h"
using jace::proxy::org::geotools::filter::text::cql2::CQLException;
#include "jace/proxy/org/geotools/filter/text/ecql/ECQL.h"
using jace::proxy::org::geotools::filter::text::ecql::ECQL;

#include "jace/proxy/mil/nga/giat/geowave/datastore/accumulo/AccumuloDataStore.h"
using jace::proxy::mil::nga::giat::geowave::datastore::accumulo::AccumuloDataStore;
#include "jace/proxy/mil/nga/giat/geowave/datastore/accumulo/metadata/AccumuloDataStatisticsStore.h"
using jace::proxy::mil::nga::giat::geowave::datastore::accumulo::metadata::AccumuloDataStatisticsStore;
#include "jace/proxy/mil/nga/giat/geowave/datastore/accumulo/metadata/AccumuloAdapterStore.h"
using jace::proxy::mil::nga::giat::geowave::datastore::accumulo::metadata::AccumuloAdapterStore;

#include "jace/proxy/mil/nga/giat/geowave/core/index/ByteArrayId.h"
using jace::proxy::mil::nga::giat::geowave::core::index::ByteArrayId;
#include "jace/proxy/mil/nga/giat/geowave/core/geotime/GeometryUtils.h"
using jace::proxy::mil::nga::giat::geowave::core::geotime::GeometryUtils;
#include "jace/proxy/mil/nga/giat/geowave/core/store/adapter/DataAdapter.h"
using jace::proxy::mil::nga::giat::geowave::core::store::adapter::DataAdapter;
#include "jace/proxy/mil/nga/giat/geowave/core/store/index/Index.h"
using jace::proxy::mil::nga::giat::geowave::core::store::index::Index;
#include "jace/proxy/mil/nga/giat/geowave/core/store/index/PrimaryIndex.h"
using jace::proxy::mil::nga::giat::geowave::core::store::index::PrimaryIndex;
#include "jace/proxy/mil/nga/giat/geowave/core/geotime/ingest/SpatialDimensionalityTypeProvider.h"
using jace::proxy::mil::nga::giat::geowave::core::geotime::ingest::SpatialDimensionalityTypeProvider;
#include "jace/proxy/mil/nga/giat/geowave/core/store/query/Query.h"
using jace::proxy::mil::nga::giat::geowave::core::store::query::Query;
#include "jace/proxy/mil/nga/giat/geowave/core/store/query/QueryOptions.h"
using jace::proxy::mil::nga::giat::geowave::core::store::query::QueryOptions;
#include "jace/proxy/mil/nga/giat/geowave/core/geotime/store/query/SpatialQuery.h"
using jace::proxy::mil::nga::giat::geowave::core::geotime::store::query::SpatialQuery;

#include "jace/proxy/mil/nga/giat/geowave/core/store/adapter/statistics/DataStatistics.h"
using jace::proxy::mil::nga::giat::geowave::core::store::adapter::statistics::DataStatistics;
#include "jace/proxy/mil/nga/giat/geowave/core/geotime/store/statistics/BoundingBoxDataStatistics.h"
using jace::proxy::mil::nga::giat::geowave::core::geotime::store::statistics::BoundingBoxDataStatistics;

#include "jace/proxy/mil/nga/giat/geowave/adapter/vector/GeotoolsFeatureDataAdapter.h"
using jace::proxy::mil::nga::giat::geowave::adapter::vector::GeotoolsFeatureDataAdapter;
#include "jace/proxy/mil/nga/giat/geowave/adapter/vector/query/cql/CQLQuery.h"
using jace::proxy::mil::nga::giat::geowave::adapter::vector::query::cql::CQLQuery;
#include "jace/proxy/mil/nga/giat/geowave/adapter/vector/plugin/ExtractGeometryFilterVisitor.h"
using jace::proxy::mil::nga::giat::geowave::adapter::vector::plugin::ExtractGeometryFilterVisitor;
#include "jace/proxy/mil/nga/giat/geowave/adapter/vector/stats/FeatureBoundingBoxStatistics.h"
using jace::proxy::mil::nga::giat::geowave::adapter::vector::stats::FeatureBoundingBoxStatistics;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

namespace csv_utils {
    const mapnik::csv_line_grammar<char const*> line_g;
    const mapnik::csv_white_space_skipper<char const*> skipper;
}

using mapnik::datasource;
using mapnik::parameters;

DATASOURCE_PLUGIN(geowave_datasource)

geowave_datasource::geowave_datasource(parameters const& params)
    :  datasource(params),
       desc_(geowave_datasource::name(), *params.get<std::string>("encoding","utf-8")),
       ctx_ (std::make_shared<mapnik::context_type>()),
       extent_(),
       zookeeper_url_(*params.get<std::string>("zookeeper_url", "")),
       instance_name_(*params.get<std::string>("instance_name", "")),
       username_(*params.get<std::string>("username", "")),
       password_(*params.get<std::string>("password", "")),
       table_namespace_(*params.get<std::string>("table_namespace", "")),
       auths_string_(*params.get<std::string>("auths", "")),
       adapter_id_(*params.get<std::string>("adapter_id", "")),
       cql_filter_(*params.get<std::string>("cql_filter", ""))
{
    this->init(params);
}

void geowave_datasource::init(parameters const& params)
{
    // Initialize JVM
    if (!jace::isRunning()){
        int status = create_jvm();
        if (status == 0)
            MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin: JVM Creation Successful";
        else
        {
            std::ostringstream err;
            err << "GeoWave Plugin: JVM Creation Failed: Error [" << status << "]";
            throw mapnik::datasource_exception(err.str());
        }
    }

    // Initialize GeoWave interface
    try 
    {
        accumulo_operations_ = java_new<BasicAccumuloOperations>(
            java_new<String>(zookeeper_url_),
            java_new<String>(instance_name_),
            java_new<String>(username_),
            java_new<String>(password_),
            java_new<String>(table_namespace_));
    }
    catch (AccumuloException& e)
    {
        std::ostringstream err;
        err << "GeoWave Plugin: There was a problem establishing a connector. " << e;
        jace::destroyVm();
        throw mapnik::datasource_exception(err.str());
    }
    catch (AccumuloSecurityException& e)
    {
        std::ostringstream err;
        err << "GeoWave Plugin: The credentials passed are invalid. " << e;
        jace::destroyVm();
        throw mapnik::datasource_exception(err.str());
    }

    AccumuloAdapterStore accumulo_adapter_store = java_new<AccumuloAdapterStore>(
        accumulo_operations_);

    DataAdapter data_adapter = accumulo_adapter_store.getAdapter(java_new<ByteArrayId>(adapter_id_));

    if (!instanceof<FeatureDataAdapter>(data_adapter))
    {
        std::ostringstream err;
        err << "GeoWave Plugin: Adapter type not supported for adapter_id: [" << adapter_id_ << "]";
        throw mapnik::datasource_exception(err.str());
    }

    feature_data_adapter_ = java_cast<FeatureDataAdapter>(data_adapter);

    // parse the user supplied authorizations
    if (!parse_auths())
        MAPNIK_LOG_DEBUG(geowave) << "Failed to parse auths from CSV line:\n" + auths_string_;

    // determine the extent of the data    
    if (!init_extent())
        MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin: There was a problem determining the extent. ";

    // initialize the layer descriptor
    init_layer_descriptor();

    // determine the geometry type
    if (!init_geometry_type())
        MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin: There was a problem determining the geometry type. ";
}

bool geowave_datasource::parse_auths()
{
    bool success = true;
    if (!auths_string_.empty())
    {
        mapnik::csv_line values;
        auto start = auths_string_.c_str();
        auto end = start + auths_string_.length();
        if (!boost::spirit::qi::phrase_parse(start, end, (csv_utils::line_g)(',', '"'), csv_utils::skipper, values))
        {
            auths_.clear();
            auths_.push_back("");
            success = false;
        } else {
            for (auto const& value : values)
            {
                auths_.push_back(value);
            }
        }
    }
    return success;
}

int geowave_datasource::create_jvm()
{
    try
    {
        StaticVmLoader loader(JNI_VERSION_1_2);

        std::string geowaveClasspath = TOSTRING(GEOWAVE_RUNTIME_JAR);

        OptionList options;
        //options.push_back(CustomOption("-Xdebug"));
        //options.push_back(CustomOption("-Xrunjdwp:server=y,transport=dt_socket,address=4000,suspend=y"));
        //options.push_back(CustomOption("-Xcheck:jni"));
        //options.push_back(Verbose (Verbose::JNI));
        //options.push_back(Verbose (Verbose::CLASS));
        options.push_back(ClassPath(geowaveClasspath));

        jace::createVm(loader, options);
    }
    catch (VirtualMachineShutdownError&)
    {
        MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin: The JVM was terminated in mid-execution. ";
        return -1;
    }
    catch (JNIException& jniException)
    {
        MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin: An unexpected JNI error has occured: " << jniException.what();
        return -2;
    }
    catch (std::exception& e)
    {
        MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin: An unexpected C++ error has occurred: " << e.what();
        return -3;
    }

    return 0;
}

bool geowave_datasource::init_extent()
{
    AccumuloDataStatisticsStore accumulo_statstore = java_new<AccumuloDataStatisticsStore>(
        accumulo_operations_);

    FeatureBoundingBoxStatistics bbox_stats = java_cast<FeatureBoundingBoxStatistics>(accumulo_statstore.getDataStatistics(
        java_new<ByteArrayId>(adapter_id_),
        FeatureBoundingBoxStatistics::composeId(feature_data_adapter_.getType().getGeometryDescriptor().getLocalName()),
        JArray<String>(auths_)));

    // pull bounds from table statistics
    Geometry stats_extent;
    if (!bbox_stats.isNull())
    {
        GeometryFactory factory = java_new<GeometryFactory>();

        JDouble lonMin = bbox_stats.getMinX();
        JDouble lonMax = bbox_stats.getMaxX();
        JDouble latMin = bbox_stats.getMinY();
        JDouble latMax = bbox_stats.getMaxY();

        JArray<Coordinate> coordArray(5);
        coordArray[0] = java_new<Coordinate>(lonMin, latMin);
        coordArray[1] = java_new<Coordinate>(lonMax, latMin);
        coordArray[2] = java_new<Coordinate>(lonMax, latMax);
        coordArray[3] = java_new<Coordinate>(lonMin, latMax);
        coordArray[4] = java_new<Coordinate>(lonMin, latMin);

        stats_extent = factory.createPolygon(coordArray);
    }

    // pull bounds from CQL statement
    Geometry cql_extent;
    if (!cql_filter_.empty())
    {
        try
        {
            filter_ = ECQL::toFilter(cql_filter_);
            cql_extent = java_cast<Geometry>(filter_.accept(ExtractGeometryFilterVisitor::GEOMETRY_VISITOR(),
                                                            Object()));
        }
        catch (CQLException& e)
        {
            MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin: Unable to parse CQL filter string." << e;
        }
    }

    // determine which bounds to use
    if (!stats_extent.isNull() && !cql_extent.isNull())
    {
        Geometry combined_extent = stats_extent;
        if (!cql_extent.equals(GeometryUtils::infinity()))
            combined_extent = cql_extent.intersection(stats_extent);

        Envelope extent = combined_extent.getEnvelopeInternal();
        extent_.init(extent.getMinX(),
                     extent.getMinY(),
                     extent.getMaxX(),
                     extent.getMaxY());
    }
    else if (!stats_extent.isNull() && cql_extent.isNull())
    {
        Envelope extent = stats_extent.getEnvelopeInternal();
        extent_.init(extent.getMinX(),
                     extent.getMinY(),
                     extent.getMaxX(),
                     extent.getMaxY());
    }
    else if (stats_extent.isNull() && !cql_extent.isNull())
    {
        Envelope extent = cql_extent.getEnvelopeInternal();
        extent_.init(extent.getMinX(),
                     extent.getMinY(),
                     extent.getMaxX(),
                     extent.getMaxY());
    }

    return extent_.valid();
}

void geowave_datasource::init_layer_descriptor()
{
    JNIEnv* env = jace::attach();
    List attribs = feature_data_adapter_.getType().getAttributeDescriptors();
    for (int i = 0; i < attribs.size(); ++i)
    {
        std::string name = java_cast<AttributeDescriptor>(attribs.get(i)).getLocalName();
        std::string class_name = java_cast<AttributeDescriptor>(attribs.get(i)).getType().getBinding().getName().replace('.', '/');
        JClassImpl attrib_class(class_name);
        
        if (env->IsAssignableFrom(attrib_class.getClass(), Geometry::staticGetJavaJniClass().getClass()))
            continue;
        else if (env->IsAssignableFrom(attrib_class.getClass(), Boolean::staticGetJavaJniClass().getClass()))
            desc_.add_descriptor(mapnik::attribute_descriptor(name, mapnik::Boolean));
        else if (env->IsAssignableFrom(attrib_class.getClass(), Integer::staticGetJavaJniClass().getClass()) || 
                 env->IsAssignableFrom(attrib_class.getClass(), Long::staticGetJavaJniClass().getClass()) || 
                 env->IsAssignableFrom(attrib_class.getClass(), Short::staticGetJavaJniClass().getClass()))
            desc_.add_descriptor(mapnik::attribute_descriptor(name, mapnik::Integer));
        else if (env->IsAssignableFrom(attrib_class.getClass(), Float::staticGetJavaJniClass().getClass()))
            desc_.add_descriptor(mapnik::attribute_descriptor(name, mapnik::Float));
        else if (env->IsAssignableFrom(attrib_class.getClass(), Double::staticGetJavaJniClass().getClass()))
            desc_.add_descriptor(mapnik::attribute_descriptor(name, mapnik::Double));
        else if (env->IsAssignableFrom(attrib_class.getClass(), String::staticGetJavaJniClass().getClass()) || 
                 env->IsAssignableFrom(attrib_class.getClass(), Date::staticGetJavaJniClass().getClass()))
            desc_.add_descriptor(mapnik::attribute_descriptor(name, mapnik::String));
        else
        {
            MAPNIK_LOG_DEBUG(geowave) << "GeoWave Plugin:  Unknown attribute type";
            continue;
        }
        ctx_->push(name);
    }
}

bool geowave_datasource::init_geometry_type()
{
    JNIEnv* env = jace::attach();
    std::string class_name = feature_data_adapter_.getType().getGeometryDescriptor().getType().getBinding().getName().replace('.', '/');
    JClassImpl geom_class(class_name);
    if (env->IsAssignableFrom(geom_class.getClass(), Point::staticGetJavaJniClass().getClass()))
        geometry_type_ =  mapnik::datasource_geometry_t::Point;
    else if (env->IsAssignableFrom(geom_class.getClass(), LineString::staticGetJavaJniClass().getClass()))
        geometry_type_ = mapnik::datasource_geometry_t::LineString;
    else if (env->IsAssignableFrom(geom_class.getClass(), Polygon::staticGetJavaJniClass().getClass()))
        geometry_type_ = mapnik::datasource_geometry_t::Polygon;
    else if (env->IsAssignableFrom(geom_class.getClass(), GeometryCollection::staticGetJavaJniClass().getClass()))
        geometry_type_ = mapnik::datasource_geometry_t::Collection;
    else
        return false;
    return true;
}

geowave_datasource::~geowave_datasource() { }

const char * geowave_datasource::name()
{
    return "geowave";
}

mapnik::datasource::datasource_t geowave_datasource::type() const
{
    return datasource::Vector;
}

mapnik::box2d<double> geowave_datasource::envelope() const
{
    return extent_;
}

boost::optional<mapnik::datasource_geometry_t> geowave_datasource::get_geometry_type() const
{
    return geometry_type_;
}

mapnik::layer_descriptor geowave_datasource::get_descriptor() const
{
    return desc_;
}

mapnik::featureset_ptr geowave_datasource::features(mapnik::query const& q) const
{
    // if the query box intersects our world extent then query for features
    mapnik::box2d<double> const& box = q.get_bbox();
    return geowave_datasource::query(box);
}

mapnik::featureset_ptr geowave_datasource::features_at_point(mapnik::coord2d const& pt, double tol) const
{
    // if the query box intersects our world extent then query for features
    mapnik::box2d<double> box(pt, pt);
    box.pad(tol);
    return geowave_datasource::query(box);
}

mapnik::featureset_ptr geowave_datasource::query(mapnik::box2d<double> const& box) const
{
    if (extent_.intersects(box))
    {
        GeometryFactory factory = java_new<GeometryFactory>();

        JDouble lonMin = box.minx();
        JDouble lonMax = box.maxx();
        JDouble latMin = box.miny();
        JDouble latMax = box.maxy();

        JArray<Coordinate> coordArray(5);
        coordArray[0] = java_new<Coordinate>(lonMin, latMin);
        coordArray[1] = java_new<Coordinate>(lonMax, latMin);
        coordArray[2] = java_new<Coordinate>(lonMax, latMax);
        coordArray[3] = java_new<Coordinate>(lonMin, latMax);
        coordArray[4] = java_new<Coordinate>(lonMin, latMin);

        AccumuloDataStore accumulo_datastore = java_new<AccumuloDataStore>(
            accumulo_operations_);

        QueryOptions query_options = java_new<QueryOptions>(feature_data_adapter_,
                                                            java_new<SpatialDimensionalityTypeProvider>().createPrimaryIndex());

        query_options.setAuthorizations(JArray<String>(auths_));

        SpatialQuery spatial_query = java_new<SpatialQuery>(factory.createPolygon(coordArray));

        CQLQuery cql_query = java_new<CQLQuery>(spatial_query,
                                                filter_,
                                                feature_data_adapter_);

        return std::make_shared<geowave_featureset>(accumulo_datastore.query(query_options,
                                                                             cql_query),
                                                    desc_.get_encoding(),
                                                    ctx_);
    }
    
    // otherwise return an empty featureset pointer
    return mapnik::featureset_ptr();
}
