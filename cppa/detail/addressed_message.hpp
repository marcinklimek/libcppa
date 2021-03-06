/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011, 2012                                                   *
 * Dominik Charousset <dominik.charousset@haw-hamburg.de>                     *
 *                                                                            *
 * This file is part of libcppa.                                              *
 * libcppa is free software: you can redistribute it and/or modify it under   *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License                  *
 * or (at your option) any later version.                                     *
 *                                                                            *
 * libcppa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 * See the GNU Lesser General Public License for more details.                *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with libcppa. If not, see <http://www.gnu.org/licenses/>.            *
\******************************************************************************/


#ifndef ADDRESSED_MESSAGE_HPP
#define ADDRESSED_MESSAGE_HPP

#include "cppa/actor.hpp"
#include "cppa/cow_tuple.hpp"
#include "cppa/channel.hpp"
#include "cppa/any_tuple.hpp"
#include "cppa/ref_counted.hpp"
#include "cppa/intrusive_ptr.hpp"

#include "cppa/detail/channel.hpp"

namespace cppa { namespace detail {

class addressed_message
{

 public:

    addressed_message(actor_ptr const& from,
            channel_ptr const& to,
            any_tuple const& ut);

    addressed_message(actor_ptr const& from,
            channel_ptr const& to,
            any_tuple&& ut);

    addressed_message() = default;
    addressed_message(addressed_message&&) = default;
    addressed_message(addressed_message const&) = default;
    addressed_message& operator=(addressed_message&&) = default;
    addressed_message& operator=(addressed_message const&) = default;

    inline actor_ptr& sender()
    {
        return m_sender;
    }

    inline actor_ptr const& sender() const
    {
        return m_sender;
    }

    inline channel_ptr& receiver()
    {
        return m_receiver;
    }

    inline channel_ptr const& receiver() const
    {
        return m_receiver;
    }

    inline any_tuple& content()
    {
        return m_content;
    }

    inline any_tuple const& content() const
    {
        return m_content;
    }

    inline bool empty() const
    {
        return m_content.empty();
    }

 private:

    actor_ptr m_sender;
    channel_ptr m_receiver;
    any_tuple m_content;

};

bool operator==(addressed_message const& lhs, addressed_message const& rhs);

inline bool operator!=(addressed_message const& lhs, addressed_message const& rhs)
{
    return !(lhs == rhs);
}

} } // namespace cppa::detail

#endif // ADDRESSED_MESSAGE_HPP
