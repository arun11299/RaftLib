/**
 * kernel.hpp - 
 * @author: Jonathan Beard
 * @version: Thu Sep 11 15:34:24 2014
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
#ifndef _KERNEL_HPP_
#define _KERNEL_HPP_  1

#include <functional>
#include <utility>
#include <cstdint>
#include <setjmp.h>

#include "port.hpp"
#include "signalvars.hpp"
#include "rafttypes.hpp"

/** pre-declare for friends **/ 
class MapBase;
class Schedule;
class kernel_container;
class Map;
class basic_parallel;

#ifndef CLONE
namespace raft
{
   class kernel;
}

#define CLONE() \
virtual raft::kernel* clone()\
{ \
   auto *ptr( \
      new typename std::remove_reference< decltype( *this ) >::type( ( *(\
   (typename std::decay< decltype( *this ) >::type * ) \
   this ) ) ) );\
   return( ptr );\
}
#endif

namespace raft {
class kernel
{
public:
   /** default constructor **/
   kernel();
   
   /** in-place allocation **/
   kernel( void * const ptr,
           const std::size_t nbytes );

   virtual ~kernel();


   /**
    * run - function to be extended for the actual execution.
    * Code can be executed outside of the run function, i.e., 
    * with any function call, however the scheduler will only 
    * call the run function so it must initiate any follow-on
    * behavior desired by the user.
    */
   virtual raft::kstatus run() = 0;
   

   template < class T /** kernel type **/,
               class ... Args >
      static kernel* make( Args&&... params )
      {
         return( new T( std::forward< Args >( params )... ) );
      }
   
   /** 
    * clone - used for parallelization of kernels, if necessary 
    * sub-kernels should include an appropriate copy 
    * constructor so all class member variables can be
    * set.
    * @param   other, T& - reference to object to be cloned
    * @return  kernel*   - takes base type, however is same as 
    * allocated by copy constructor for T.
    */
   virtual raft::kernel* clone()
   {
      //FIXME, needs to throw an exception
      assert( false );
      return( nullptr );
   }

   std::size_t get_id();
protected:
   /**
    * 
    */
   virtual std::size_t addPort();
   
   virtual void lock();
   virtual void unlock();

   /**
    * PORTS - input and output, use these to interact with the
    * outside world.
    */
   Port               input  = { this };
   Port               output = { this };
 

   friend class ::MapBase;
   friend class ::Map;
   friend class ::Schedule;
   friend class ::GraphTools;
   friend class ::kernel_container;   
   friend class ::basic_parallel;

   /**
    * NOTE: doesn't need to be atomic since only one thread
    * will have responsibility to to create new compute 
    * kernels.
    */
   static std::size_t kernel_count;

private:
   /** TODO, replace dup with bit vector **/
   bool   dup_enabled   = false;
   bool   dup_candidate = false;
   const  std::size_t kernel_id;
};
} /** end namespace raft */
#endif /* END _KERNEL_HPP_ */
