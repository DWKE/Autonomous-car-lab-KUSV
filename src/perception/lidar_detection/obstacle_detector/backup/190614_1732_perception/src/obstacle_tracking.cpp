#include "obstacle_tracking.hpp"

ObstacleTracking::ObstacleTracking()
{
	m_MAX_ASSOCIATION_DISTANCE = 2.0;
	iTracksNumber = 0;
}


ObstacleTracking::~ObstacleTracking() {}


void ObstacleTracking::associate (const std::vector<clusterPtr>& objList)
{
	m_DetectedObjects = objList;

	matchWithDistanceOnly();
}


void ObstacleTracking::matchWithDistanceOnly()
{
	m_TmpTrackingObjects.clear();
	double d_y = 0, d_x = 0, d = 0;


	while (m_DetectedObjects.size() > 0)
	{
		int iClosest_track = -1;
		int iClosest_obj = -1;
		double dClosest = m_MAX_ASSOCIATION_DISTANCE;
		double size_diff = 0;
		//std::cout << std::endl;

		// foreach detected object pointer
		unsigned int objectNum = 0;
		for (const auto& pDetectedObject : m_DetectedObjects)
		{
			double object_size = sqrt (pow(pDetectedObject->m_width, 2.0) + pow(pDetectedObject->m_length, 2.0) + pow(pDetectedObject->m_height, 2.0));

			unsigned int trackNum = 0;
			for (const auto& pTrackingObject : m_TrackingObjects)
			{
				double old_size = sqrt (pow(pTrackingObject->m_width, 2.0) + pow(pTrackingObject->m_height, 2.0) + pow(pTrackingObject->m_length, 2.0));

				d_y = pDetectedObject->m_center.position.y - pTrackingObject->m_center.position.y;
				d_x = pDetectedObject->m_center.position.x - pTrackingObject->m_center.position.x;
				d = hypot(d_y, d_x);

				if(d < dClosest)
				{
					dClosest = d;
					iClosest_obj = objectNum;
					iClosest_track = trackNum;
					size_diff = fabs(old_size - object_size);
				}
				trackNum++;
			}
			objectNum++;
		}

		if(iClosest_obj != -1 && iClosest_track != -1 && dClosest < m_MAX_ASSOCIATION_DISTANCE)
		{
			//std::cout << "MatchObj: " << m_TrackingObjects.at(iClosest_track)->m_id << ", MinD: " << dClosest << ", SizeDiff: (" << size_diff <<  ")" << ", ObjI " << iClosest_obj <<", TrackI: " << iClosest_track << std::endl;

			m_DetectedObjects.at(iClosest_obj)->m_id = m_TrackingObjects.at(iClosest_track)->m_id;
			MergeObjectAndTrack(m_TrackingObjects.at(iClosest_track), m_DetectedObjects.at(iClosest_obj));

			m_TmpTrackingObjects.push_back(m_TrackingObjects.at(iClosest_track));

			m_TrackingObjects.erase(m_TrackingObjects.begin()+iClosest_track);
			m_DetectedObjects.erase(m_DetectedObjects.begin()+iClosest_obj);
		}
		else
		{
			iTracksNumber += 1;
			m_DetectedObjects.at(0)->m_id = iTracksNumber;
			m_TmpTrackingObjects.push_back(m_DetectedObjects.at(0));
			m_DetectedObjects.erase (m_DetectedObjects.begin() + 0);

		}
	}

	m_TrackingObjects = m_TmpTrackingObjects;
}


void ObstacleTracking::MergeObjectAndTrack (clusterPtr& track, clusterPtr& obj)
{
	track = obj;
}
