"""
 Copyright (C) Elektrobit Automotive GmbH
 All rights exclusively reserved for Elektrobit Automotive GmbH,
 unless expressly agreed to otherwise.
"""

from rtegraph import graph_gen_utils
from rtegraph.constants import *

RUNNABLE_BY_ID = "partitions/*/softwareComponents/*/runnables/*[@id='{}']"
BSW_MOODULE_BY_ID = "partitions/*/bswModules/*/bsnps/*/bswModuleEntities/*[@id='{}']"
RUNNABLE_BSW_PROPERTIES = {'name':"Name", 'entryPoint':"entryPoint"}
TASK_OF_NAME = "partitions/*/tasks/*[name='{}']"
INTERRUPT_OF_NAME = "partitions/*/Interrupts/*[name='{}']"
EVENTS = "events/*"
TASK_PROPERTIES = {'name':"Name", 'priority':"priority", 'configuredPriority':"Configured Priority"}
NAME = "name"
COMPONENT_CLUSTER_PROPERTIES = "{}\nCore: {}\nPartition: {}"
CORE_ID = 'coreId'
MAPPED_TO_TASK = 'mappedToTask'
TASK_EVENT_MAPPING = "taskEventMappings/*"
EVENT_NAME = 'eventName'
EXECUTABLE_REF = 'executableRef'
TASK_NAME = 'taskName'

def __addExecutable(root, executable_instance_ref, cluster, destnode, connectionlabel = None):
    executable_instance = root.xpath(RUNNABLE_BY_ID.format(executable_instance_ref))
    if len(executable_instance) == 0:
        executable_instance = root.xpath(BSW_MOODULE_BY_ID.format(executable_instance_ref))
        swc_bsw = executable_instance[0].getparent().getparent().getparent().getparent() #to get the BSW xml element
    else:
        swc_bsw = executable_instance[0].getparent().getparent() #to get the SWC xml element

    swc_bsw_name = swc_bsw.find(NAME).text
    partition = swc_bsw.getparent().getparent()

    subcluster = graph_gen_utils.new_cluster(swc_bsw_name + cluster.id, 
                COMPONENT_CLUSTER_PROPERTIES.format(swc_bsw_name, partition.find(CORE_ID).text, partition.find(NAME).text))
    if subcluster.id not in cluster.clusters.keys():
        cluster.add_cluster(subcluster)
    else:
        subcluster = cluster.clusters[subcluster.id]
    exexutableNode = graph_gen_utils.get_or_create_node(subcluster, executable_instance[0], RUNNABLE_BSW_PROPERTIES, RECORD, LIGHT_BLUE)
    graph_gen_utils.connect(exexutableNode, destnode, cluster, connectionlabel)


def main():
    root = graph_gen_utils.get_model_root()
    for event in root.xpath(EVENTS):
        for mapping in event.xpath(TASK_EVENT_MAPPING):
            if mapping.find(MAPPED_TO_TASK).text == 'true':
                eventname = mapping.find(EVENT_NAME).text
                taskname = mapping.find(TASK_NAME).text
                task = root.xpath(TASK_OF_NAME.format(taskname))
                if len(task) == 0:
                    # will check if it is mapped to an ISR
                    task = root.xpath(INTERRUPT_OF_NAME.format(mapping.find(TASK_NAME).text))
                cluster = graph_gen_utils.get_or_create_cluster(taskname, taskname)
                taskNode = graph_gen_utils.get_or_create_node(cluster, task[0], TASK_PROPERTIES, RECORD, YELLOW)
            else:
                eventname = mapping.find(EVENT_NAME).text
                taskname = 'Unknown'
                cluster = graph_gen_utils.get_or_create_cluster(taskname, taskname)
                taskNode = graph_gen_utils.get_or_create_node_with_customized_id('unknownTask' + cluster.id, cluster, shape=RECORD, color=BROWN, additionaltext='Unknown Task')
            eventNode = graph_gen_utils.get_or_create_node_with_customized_id(eventname + taskname, cluster, mapping.find(EVENT_NAME), {'.':'.'}, None)
            graph_gen_utils.connect(eventNode, taskNode, cluster, '')
            __addExecutable(root, mapping.find(EXECUTABLE_REF).text, cluster, eventNode, "")

    # Add legends
    graph_gen_utils.add_legend('task', YELLOW, RECORD, BLACK)
    graph_gen_utils.add_legend('runnable', LIGHT_BLUE, RECORD, BLACK)
    graph_gen_utils.add_legend('event', WHITE, 'ellipse', BLACK)

    # generate graph
    graph_gen_utils.generate_graph("Event to task mappings group by task", "event-to-task-mappings-group-by-task")


if __name__=="__main__":
    args = graph_gen_utils.process_args()
    main()
