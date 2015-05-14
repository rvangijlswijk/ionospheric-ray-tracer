/*
 * GeometryType.h
 *
 *  Created on: 15 Apr 2015
 *      Author: rian
 */

#ifndef SCENE_GEOMETRYTYPE_H_
#define SCENE_GEOMETRYTYPE_H_

namespace raytracer {
namespace scene {

	enum GeometryType {
		none = 0,
		ionosphere = 1,
		terrain = 2,
		atmosphere = 3
	};

}}

#endif /* SCENE_GEOMETRYTYPE_H_ */
