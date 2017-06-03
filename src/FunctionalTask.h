/*--------------------------------------------------------------------
Task is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

Task is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

See GNU Lesser General Public License at <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/
#pragma once

#include <Task.h>
#include <functional>

class FunctionalTask : public Task
{
public:
    typedef std::function<void(uint32_t)> functionCallback;

    FunctionalTask(const functionCallback &function, uint32_t timeInterval) :
            Task(timeInterval)
    {
        this->_callback = function;
    };

private:
    functionCallback _callback;

    virtual void OnUpdate(uint32_t deltaTime)
    {
        if(_callback)
            _callback(deltaTime);
    }
};

