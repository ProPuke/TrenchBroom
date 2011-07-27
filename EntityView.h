//
//  EntityView.h
//  TrenchBroom
//
//  Created by Kristian Duske on 15.07.11.
//  Copyright 2011 TU Berlin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EntityDefinitionManager.h"

@class GLResources;
@class EntityDefinitionLayout;
@class EntityDefinition;
@protocol EntityDefinitionFilter;

@interface EntityView : NSOpenGLView {
    NSPoint dragDistance;
    EntityDefinition* draggedEntityDefinition;
    EntityDefinitionManager* entityDefinitionManager;
    GLResources* glResources;
    EntityDefinitionLayout* layout;
    IBOutlet id target;
    id <EntityDefinitionFilter> filter;
    EEntityDefinitionSortCriterion sortCriterion;
}

- (void)setGLResources:(GLResources *)theGLResources entityDefinitionManager:(EntityDefinitionManager *)theEntityDefinitionManager;
- (void)setEntityDefinitionFilter:(id <EntityDefinitionFilter>)theFilter;
- (void)setSortCriterion:(EEntityDefinitionSortCriterion)criterion;

@end