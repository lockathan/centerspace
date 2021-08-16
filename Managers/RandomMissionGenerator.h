#ifndef RandomMissionGenerator_H
#define RandomMissionGenerator_H


class BattlefieldGrid;
class FactionHubTemplate;
class CampaignState;
class Mission;

class RandomMissionGenerator
{
public:
	~RandomMissionGenerator();

	Mission* generateMission(CampaignState* state, int missionId);
	void finalizeMission(Mission* mission, CampaignState* state, BattlefieldGrid* grid);

	static RandomMissionGenerator* getSingletonPtr();
protected:
	static RandomMissionGenerator* mRandomMissionGenerator;
private:
	RandomMissionGenerator();
	RandomMissionGenerator( const RandomMissionGenerator& ) { }
	RandomMissionGenerator & operator = ( const RandomMissionGenerator& );

};
#endif