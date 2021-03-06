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

#ifndef __TrenchBroom__BoxInfoRenderer__
#define __TrenchBroom__BoxInfoRenderer__

#include "Renderer/Text/TextRenderer.h"
#include "Utility/VecMath.h"

using namespace TrenchBroom::VecMath;

namespace TrenchBroom {
    namespace Renderer {
        namespace Text {
            class FontManager;
        }
        
        class Camera;
        class RenderContext;
        class Vbo;
        
        class BoxInfoSizeTextAnchor : public Text::TextAnchor {
        private:
            BBoxf m_bounds;
            Axis::Type m_axis;
            Renderer::Camera& m_camera;
        protected:
            const Vec3f basePosition() const;
            const Text::Alignment::Type alignment() const;
        public:
            BoxInfoSizeTextAnchor(const BBoxf& bounds, Axis::Type axis, Renderer::Camera& camera);
        };
        
        class BoxInfoMinMaxTextAnchor : public Text::TextAnchor {
        public:
            typedef enum {
                BoxMin,
                BoxMax
            } EMinMax;
        private:
            BBoxf m_bounds;
            EMinMax m_minMax;
            Renderer::Camera& m_camera;
        protected:
            const Vec3f basePosition() const;
            const Text::Alignment::Type alignment() const;
        public:
            BoxInfoMinMaxTextAnchor(const BBoxf& bounds, EMinMax minMax, Renderer::Camera& camera);
        };

        class BoxInfoRenderer {
        private:
            BBoxf m_bounds;
            Text::TextRenderer<unsigned int>* m_textRenderer;
            Text::TextRenderer<unsigned int>::SimpleTextRendererFilter m_textFilter;
            bool m_initialized;
            
            // prevent copying
            BoxInfoRenderer(const BoxInfoRenderer& other);
            void operator= (const BoxInfoRenderer& other);
        public:
            BoxInfoRenderer(const BBoxf& bounds, Text::FontManager& fontManager);
            ~BoxInfoRenderer();
            
            void render(Vbo& vbo, RenderContext& context);
        };
    }
}

#endif /* defined(__TrenchBroom__BoxInfoRenderer__) */
