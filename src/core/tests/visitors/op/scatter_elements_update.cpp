// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/op/scatter_elements_update.hpp"

#include "gtest/gtest.h"
#include "ngraph/ngraph.hpp"
#include "ngraph/op/util/attr_types.hpp"
#include "util/visitor.hpp"

using namespace ov;
using ngraph::test::NodeBuilder;
using ngraph::test::ValueMap;

TEST(attributes, scatter_elements_update) {
    NodeBuilder::get_ops().register_factory<op::v3::ScatterElementsUpdate>();

    auto data = std::make_shared<op::v0::Parameter>(element::f32, Shape{2, 4, 5, 7});
    auto indices = std::make_shared<op::v0::Parameter>(element::i16, Shape{2, 2, 2, 2});
    auto updates = std::make_shared<op::v0::Parameter>(element::f32, Shape{2, 2, 2, 2});
    auto axis = std::make_shared<op::v0::Parameter>(element::i16, Shape{});

    auto scatter = std::make_shared<op::v3::ScatterElementsUpdate>(data, indices, updates, axis);
    NodeBuilder builder(scatter, {data, indices, updates, axis});

    const auto expected_attr_count = 0;
    EXPECT_EQ(builder.get_value_map_size(), expected_attr_count);
}

TEST(attributes, scatter_elements_update_v12) {
    NodeBuilder::get_ops().register_factory<op::v12::ScatterElementsUpdate>();

    auto data = std::make_shared<op::v0::Parameter>(element::f32, Shape{2, 4, 5, 7});
    auto indices = std::make_shared<op::v0::Parameter>(element::i16, Shape{2, 2, 2, 2});
    auto updates = std::make_shared<op::v0::Parameter>(element::f32, Shape{2, 2, 2, 2});
    auto axis = std::make_shared<op::v0::Parameter>(element::i16, Shape{});

    auto scatter = std::make_shared<op::v12::ScatterElementsUpdate>(data,
                                                                    indices,
                                                                    updates,
                                                                    axis,
                                                                    op::v12::ScatterElementsUpdate::Reduction::PROD,
                                                                    false);
    NodeBuilder builder(scatter, {data, indices, updates, axis});
    const auto g_scatter = ov::as_type_ptr<op::v12::ScatterElementsUpdate>(builder.create());

    const auto expected_attr_count = 2;
    EXPECT_EQ(builder.get_value_map_size(), expected_attr_count);
    EXPECT_EQ(g_scatter->get_reduction(), scatter->get_reduction());
    EXPECT_EQ(g_scatter->get_use_init_val(), scatter->get_use_init_val());
}
