// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "probability_util.h"

RareErrorIterator::RareErrorIterator(float probability)
    : next_candidate(0), is_one(probability == 1), dist(probability) {
    if (!(probability >= 0 && probability <= 1)) {
        throw std::out_of_range("Invalid probability.");
    }
}

size_t RareErrorIterator::next(std::mt19937_64 &rng) {
    size_t result = next_candidate + (is_one ? 0 : dist(rng));
    next_candidate = result + 1;
    return result;
}

std::vector<size_t> sample_hit_indices(float probability, size_t attempts, std::mt19937_64 &rng) {
    std::vector<size_t> result;
    RareErrorIterator::for_samples(probability, attempts, rng, [&](size_t s) {
        result.push_back(s);
    });
    return result;
}

std::mt19937_64 externally_seeded_rng() {
    std::random_device d;
    std::seed_seq seq{d(), d(), d(), d(), d(), d(), d(), d()};
    std::mt19937_64 result(seq);
    return result;
}
