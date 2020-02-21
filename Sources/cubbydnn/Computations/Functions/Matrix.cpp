// Copyright (c) 2019 Chris Ohk, Justin Kim

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <cubbydnn/computations/Functions/Matrix.hpp>
#include <cubbydnn/computations/Functions/MatrixOps.hpp>

namespace CubbyDNN
{
void IdentityMatrix(const Shape& shape, NumberSystem numberSystem)
{
    Tensor tensor = AllocateTensor({ shape, numberSystem });
    switch (numberSystem)
    {
        case NumberSystem::Float32:
            GetIdentityMatrix<float>(tensor);
        case NumberSystem::Int32:
            GetIdentityMatrix<int>(tensor);
        default:
            assert(false && "NotImplementedError");
    }
}

void Multiply(const Tensor& inputA, const Tensor& inputB, Tensor& output)
{
    assert(inputA.Info.GetNumberSystem() == inputB.Info.GetNumberSystem() &&
        inputA.Info.GetNumberSystem() == output.Info.GetNumberSystem());

    const auto numberSystem = inputA.Info.GetNumberSystem();
    switch (numberSystem)
    {
        case NumberSystem::Float32:
            MatMul<float>(inputA, inputB, output);
        case NumberSystem::Int32:
            MatMul<int>(inputA, inputB, output);
        default:
            assert(false && "NotImplementedError");
    }
}
} // namespace CubbyDNN
