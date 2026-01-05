import torch
train_on_gpu = torch.cuda.is_available()
if train_on_gpu:
    print("CUDA is available, Training on GPU ...")
else:
    print("CUDA is not available!  Training on CPU ...")
