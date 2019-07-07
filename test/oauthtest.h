namespace GS {
class OAuth2;
}

namespace GSTest
{
class OAuthTest {

public:
  int SetUp() const;
  bool ClientIDCorrect(const GS::OAuth2& oauth) const;
};
}
