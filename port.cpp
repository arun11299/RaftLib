/**
 * port.cpp - 
 * @author: Jonathan Beard
 * @version: Thu Aug 28 09:55:47 2014
 * 
 * Copyright 2014 Jonathan Beard
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <algorithm>
#include <cassert>

#include "port.hpp"
#include "portexception.hpp"

Port::Port()
{
}

Port::~Port()
{
}

const type_info&
Port::getPortType( const std::string port_name )
{
   const auto ret_val( portmap.find( port_name ) );
   if( ret_val == portmap.cend() )
   {
      throw PortNotFoundException( "Port not found for name \"" + port_name + "\"" );
   }
   return( ret_val.second.type );
}

FIFO&
Port::operator[]( const std::string port_name )
{
   const auto ret_val( portmap.find( port_name ) );
   if( ret_val == portmap.cend() )
   {
      throw PortNotFoundException( "Port not found for name \"" + port_name + "\"" );
   }
   return( *((*ret_val).fifo)  );
}

void
Port::initializePort( const std::string port_name,
                      FIFO             *fifo )
{
   assert( fifo != std::nullptr );
   const auto ret_val( portmap.find( port_name ) );
   if( ret_val == portmap.cend() )
   {
      throw  PortNotFoundException( "Port not found for name \"" + port_name + "\"" );
   }
   if( (*ret_val).fifo != std::nullptr )
   {
      throw PortDoubleInitializeException( 
         "Port \"" + port_name "\" already initialized!" );
   }
   /** else initialize **/
   (*ret_val).fifo = fifo;
}