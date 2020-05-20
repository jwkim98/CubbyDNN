/// Copyright (c) 2019 Chris Ohk, Justin Kim

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYDNN_OPTIMIZER_HPP
#define CUBBYDNN_OPTIMIZER_HPP

#include <cubbydnn/Tensors/Tensor.hpp>

namespace CubbyDNN::Computation
{
class Optimizer
{
public:
    Optimizer();
    virtual ~Optimizer();

    Optimizer(const Optimizer& optimizer) = default;
    Optimizer(Optimizer&& optimizer) noexcept = default;
    Optimizer& operator=(const Optimizer& optimizer) = default;
    Optimizer& operator=(Optimizer&& optimizer) noexcept = default;

    virtual void Optimize(Tensor& tensor) = 0;
};
}

#endif