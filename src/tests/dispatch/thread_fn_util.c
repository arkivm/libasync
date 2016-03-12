#include "thread_fn_util.h"

static inline int send_and_get_response(
	struct fipc_ring_channel *chan,
	struct fipc_message *request,
	struct fipc_message **response,
    uint32_t msg_id)
{
	int ret;
	struct fipc_message *resp;

	/*
	 * Mark the request as sent
	 */
	ret = fipc_send_msg_end(chan, request);
	if (ret) {
		pr_err("failed to mark request as sent, ret = %d\n", ret);
		goto fail1;
	}
	/*
	 * Try to get the response
	 */
    ret = thc_ipc_recv(chan, msg_id, &resp);
	if (ret) {
		pr_err("failed to get a response, ret = %d\n", ret);
		goto fail2;
	}
	*response = resp;
    printk(KERN_ERR "got result\n");

	return 0;

fail2:
fail1:
	return ret;
}