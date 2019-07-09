#include <QString>

namespace GS
{
struct AppManifest {
  static const QString OrgName;
  static const QString AppName;
  static const QString Version;
};
}

const QString GS::AppManifest::OrgName{"Sonic Original Software"};
const QString GS::AppManifest::AppName{"GamerSaver"};
const QString GS::AppManifest::Version{"0.1.0"};

