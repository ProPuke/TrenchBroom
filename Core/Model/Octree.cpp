/*
 Copyright (C) 2010-2012 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Octree.h"
#include <algorithm>
#include <cmath>
#include <assert.h>
#include "Entity.h"
#include "EntityDefinition.h"
#include "Brush.h"
#include "Map.h"

namespace TrenchBroom {
    namespace Model {
        bool OctreeNode::addObject(MapObject& object, int childIndex) {
            if (m_children[childIndex] == NULL) {
                BBox childBounds;
                switch (childIndex) {
                    case CP_WSB:
                        childBounds.min.x = m_bounds.min.x;
                        childBounds.min.y = m_bounds.min.y;
                        childBounds.min.z = m_bounds.min.z;
                        childBounds.max.x = (m_bounds.min.x + m_bounds.max.x) / 2;
                        childBounds.max.y = (m_bounds.min.y + m_bounds.max.y) / 2;
                        childBounds.max.z = (m_bounds.min.z + m_bounds.max.z) / 2;
                        break;
                    case CP_WST:
                        childBounds.min.x = m_bounds.min.x;
                        childBounds.min.y = m_bounds.min.y;
                        childBounds.min.z = (m_bounds.min.z + m_bounds.max.z) / 2;
                        childBounds.max.x = (m_bounds.min.x + m_bounds.max.x) / 2;
                        childBounds.max.y = (m_bounds.min.y + m_bounds.max.y) / 2;
                        childBounds.max.z = m_bounds.max.z;
                        break;
                    case CP_WNB:
                        childBounds.min.x = m_bounds.min.x;
                        childBounds.min.y = (m_bounds.min.y + m_bounds.max.y) / 2;
                        childBounds.min.z = m_bounds.min.z;
                        childBounds.max.x = (m_bounds.min.x + m_bounds.max.x) / 2;
                        childBounds.max.y = m_bounds.max.y;
                        childBounds.max.z = (m_bounds.min.z + m_bounds.max.z) / 2;
                        break;
                    case CP_ESB:
                        childBounds.min.x = (m_bounds.min.x + m_bounds.max.x) / 2;
                        childBounds.min.y = m_bounds.min.y;
                        childBounds.min.z = m_bounds.min.z;
                        childBounds.max.x = m_bounds.max.x;
                        childBounds.max.y = (m_bounds.min.y + m_bounds.max.y) / 2;
                        childBounds.max.z = (m_bounds.min.z + m_bounds.max.z) / 2;
                        break;
                    case CP_EST:
                        childBounds.min.x = (m_bounds.min.x + m_bounds.max.x) / 2;
                        childBounds.min.y = m_bounds.min.y;
                        childBounds.min.z = (m_bounds.min.z + m_bounds.max.z) / 2;
                        childBounds.max.x = m_bounds.max.x;
                        childBounds.max.y = (m_bounds.min.y + m_bounds.max.y) / 2;
                        childBounds.max.z = m_bounds.max.z;
                        break;
                    case CP_ENB:
                        childBounds.min.x = (m_bounds.min.x + m_bounds.max.x) / 2;
                        childBounds.min.y = (m_bounds.min.y + m_bounds.max.y) / 2;
                        childBounds.min.z = m_bounds.min.z;
                        childBounds.max.x = m_bounds.max.x;
                        childBounds.max.y = m_bounds.max.y;
                        childBounds.max.z = (m_bounds.min.z + m_bounds.max.z) / 2;
                        break;
                    case CP_ENT:
                        childBounds.min.x = (m_bounds.min.x + m_bounds.max.x) / 2;
                        childBounds.min.y = (m_bounds.min.y + m_bounds.max.y) / 2;
                        childBounds.min.z = (m_bounds.min.z + m_bounds.max.z) / 2;
                        childBounds.max.x = m_bounds.max.x;
                        childBounds.max.y = m_bounds.max.y;
                        childBounds.max.z = m_bounds.max.z;
                        break;
                }
                m_children[childIndex] = new OctreeNode(childBounds, m_minSize);
            }
            return m_children[childIndex]->addObject(object);
        }

        OctreeNode::OctreeNode(const BBox& bounds, int minSize) : m_bounds(bounds), m_minSize(minSize) {
            for (int i = 0; i < 8; i++)
                m_children[i] = NULL;
        }
        
        OctreeNode::~OctreeNode() {
            for (int i = 0; i < 8; i++)
                if (m_children != NULL)
                    delete m_children[i];
        }
        
        bool OctreeNode::addObject(MapObject& object) {
            if (!m_bounds.contains(object.bounds())) return false;
            if (m_bounds.max.x - m_bounds.min.x > m_minSize)
                for (int i = 0; i < 8; i++)
                    if (addObject(object, i)) return true;
            m_objects.push_back(&object);
            return true;
        }
        
        bool OctreeNode::removeObject(MapObject& object) {
            if (!m_bounds.contains(object.bounds())) return false;
            for (int i = 0; i < 8; i++)
                if (m_children[i] != NULL && m_children[i]->removeObject(object)) return true;
            vector<MapObject*>::iterator it = find(m_objects.begin(), m_objects.end(), &object);
            if (it != m_objects.end()) m_objects.erase(it);
            return true;
        }
        
        void OctreeNode::intersect(const Ray& ray, vector<MapObject*>& objects) {
            if (m_bounds.contains(ray.origin) || !std::isnan(m_bounds.intersectWithRay(ray))) {
                objects.insert(objects.end(), m_objects.begin(), m_objects.end());
                for (int i = 0; i < 8; i++)
                    if (m_children[i] != NULL) m_children[i]->intersect(ray, objects);
            }
        }
        
        void Octree::entitiesWereAddedOrPropertiesDidChange(const vector<Entity*>& entities) {
            for (int i = 0; i < entities.size(); i++) {
                Entity* entity = entities[i];
                if (entity->entityDefinition() != NULL && entity->entityDefinition()->type == EDT_POINT)
                    m_root->addObject(*entity);
            }
        }
        
        void Octree::entitiesWillBeRemovedOrPropertiesWillChange(const vector<Entity*>& entities){
            for (int i = 0; i < entities.size(); i++) {
                Entity* entity = entities[i];
                if (entity->entityDefinition() != NULL && entity->entityDefinition()->type == EDT_POINT)
                    assert(m_root->removeObject(*entity));
            }
        }
        
        void Octree::brushesWereAddedOrDidChange(const vector<Brush*>& brushes) {
            for (int i = 0; i < brushes.size(); i++) {
                Brush* brush = brushes[i];
                m_root->addObject(*brush);
            }
        }
        
        void Octree::brushesWillBeRemovedOrWillChange(const vector<Brush*>& brushes) {
            for (int i = 0; i < brushes.size(); i++) {
                Brush* brush = brushes[i];
                assert(m_root->removeObject(*brush));
            }
        }
        
        void Octree::mapLoaded(Map& map) {
            const vector<Entity*>& entities = map.entities();
            for (int i = 0; i < entities.size(); i++) {
                Entity* entity = entities[i];
                if (entity->entityDefinition() != NULL && entity->entityDefinition()->type == EDT_POINT)
                    m_root->addObject((MapObject&)*entity);
                const vector<Brush*>& brushes = entity->brushes();
                for (int j = 0; j < brushes.size(); j++) {
                    Brush* brush = brushes[j];
                    m_root->addObject(*brush);
                }
            }
        }
        
        void Octree::mapCleared(Map& map) {
            delete m_root;
            m_root = new OctreeNode(m_map.worldBounds(), m_minSize);
        }
        
        Octree::Octree(Map& map, int minSize) : m_minSize(minSize), m_map(map), m_root(new OctreeNode(map.worldBounds(), minSize)) {
            m_map.entitiesWereAdded     += new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWereAddedOrPropertiesDidChange);
            m_map.entitiesWillBeRemoved += new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWillBeRemovedOrPropertiesWillChange);
            m_map.propertiesWillChange  += new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWillBeRemovedOrPropertiesWillChange);
            m_map.propertiesDidChange   += new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWereAddedOrPropertiesDidChange);
            m_map.brushesWereAdded      += new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWereAddedOrDidChange);
            m_map.brushesWillBeRemoved  += new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWillBeRemovedOrWillChange);
            m_map.brushesWillChange     += new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWillBeRemovedOrWillChange);
            m_map.brushesDidChange      += new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWereAddedOrDidChange);
            m_map.mapLoaded             += new Model::Map::MapEvent::T<Octree>(this, &Octree::mapLoaded);
            m_map.mapCleared            += new Model::Map::MapEvent::T<Octree>(this, &Octree::mapCleared);
        }
        
        Octree::~Octree() {
            m_map.entitiesWereAdded     -= new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWereAddedOrPropertiesDidChange);
            m_map.entitiesWillBeRemoved -= new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWillBeRemovedOrPropertiesWillChange);
            m_map.propertiesWillChange  -= new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWillBeRemovedOrPropertiesWillChange);
            m_map.propertiesDidChange   -= new Model::Map::EntityEvent::T<Octree>(this, &Octree::entitiesWereAddedOrPropertiesDidChange);
            m_map.brushesWereAdded      -= new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWereAddedOrDidChange);
            m_map.brushesWillBeRemoved  -= new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWillBeRemovedOrWillChange);
            m_map.brushesWillChange     -= new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWillBeRemovedOrWillChange);
            m_map.brushesDidChange      -= new Model::Map::BrushEvent::T<Octree>(this, &Octree::brushesWereAddedOrDidChange);
            m_map.mapLoaded             -= new Model::Map::MapEvent::T<Octree>(this, &Octree::mapLoaded);
            m_map.mapCleared            -= new Model::Map::MapEvent::T<Octree>(this, &Octree::mapCleared);
            delete m_root;
        }
        
        vector<MapObject*> Octree::intersect(const Ray& ray) {
            vector<MapObject*> result;
            m_root->intersect(ray, result);
            return result;
        }
    }
}